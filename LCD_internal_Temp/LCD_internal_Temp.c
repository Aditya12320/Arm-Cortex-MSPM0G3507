/*
 * Include necessary header files
 */
#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdint.h>
#include "ti/driverlib/dl_i2c.h"
#include <ti/iqmath/include/IQmathLib.h>

/* I2C LCD address (commonly 0x27 or 0x3F for I2C LCD modules) */
#define LCD_I2C_ADDRESS 0x27

/* LCD row addresses */
#define LCD_LINE_1 0x80  // Address of the first character on line 1
#define LCD_LINE_2 0xC0  // Address of the first character on line 2

/* Function prototypes */
void I2C_init(void);
void LCD_init(void);
void LCD_sendCommand(uint8_t cmd);
void LCD_sendData(uint8_t data);
void LCD_sendString(char *str);
void LCD_setCursor(uint8_t row, uint8_t col);
void delay_ms(uint32_t ms);
void LCD_sendByte(uint8_t byte, uint8_t mode);
void I2C_writeByte(uint8_t data);
int temperature(void);

/* Temperature-related functions */
#define TEMP_TS_TRIM_C ((uint32_t)30)
#define TEMP_TS_COEF_mV_C (-555.55f)
#define ADC_VREF_VOLTAGE (3.3f)
#define ADC_BIT_RESOLUTION ((uint32_t)(1)<<12)

volatile bool gCheckADC;
volatile float gTemperatureDegC;
volatile float gTemperatureDegF;

/* Main function */
int main(void)
{
    SYSCFG_DL_init();

    /* Initialize I2C module */
    I2C_init();

    /* Initialize LCD */
    LCD_init();

    /* Start temperature sensor function */
    temperature();

    while (1) {
        /* Infinite loop */
    }
}

/* Initialize the I2C module */
void I2C_init(void)
{
    /* Initialize I2C with 100kHz speed */
    DL_I2C_enableController(I2C_INST);
}

/* Function to initialize the LCD in 4-bit mode */
void LCD_init(void)
{
    delay_ms(50);

    LCD_sendCommand(0x33); // Initialize to 8-bit mode first
    LCD_sendCommand(0x32); // Then switch to 4-bit mode
    LCD_sendCommand(0x28); // Function set: 4-bit, 2 lines, 5x8 font
    LCD_sendCommand(0x0C); // Display ON, cursor OFF
    LCD_sendCommand(0x06); // Entry mode set: Increment cursor, no shift
    LCD_sendCommand(0x01); // Clear display
    delay_ms(2);
}

/* Function to send a command to the LCD */
void LCD_sendCommand(uint8_t cmd)
{
    uint8_t upper_nibble = cmd & 0xF0;            // High nibble
    uint8_t lower_nibble = (cmd << 4) & 0xF0;     // Low nibble

    LCD_sendByte(upper_nibble, 0);
    delay_ms(2);

    LCD_sendByte(lower_nibble, 0);
    delay_ms(2);
}

/* Function to send data to the LCD */
void LCD_sendData(uint8_t data)
{
    uint8_t upper_nibble = data & 0xF0;
    uint8_t lower_nibble = (data << 4) & 0xF0;

    LCD_sendByte(upper_nibble, 1);
    LCD_sendByte(lower_nibble, 1);
}

/* Function to send a byte to the LCD with control bits */
void LCD_sendByte(uint8_t byte, uint8_t mode)
{
    uint8_t data;
    data = byte | (mode ? 0x01 : 0x00); // RS bit
    data |= 0x08; // Enable backlight

    I2C_writeByte(data | 0x04); // En=1
    delay_ms(1);
    I2C_writeByte(data & ~0x04); // En=0
    delay_ms(1);
}

/* Function to set the cursor to a specific row and column */
void LCD_setCursor(uint8_t row, uint8_t col)
{
    uint8_t address;
    switch (row)
    {
        case 1:
            address = LCD_LINE_1 + (col - 1);
            break;
        case 2:
            address = LCD_LINE_2 + (col - 1);
            break;
        default:
            address = LCD_LINE_1;
            break;
    }
    LCD_sendCommand(address);
}

/* Function to send a string to the LCD */
void LCD_sendString(char *str)
{
    while (*str) {
        LCD_sendData(*str++);
    }
}

/* Function to write a byte to the I2C bus */
void I2C_writeByte(uint8_t data)
{
    uint8_t txData[1];
    txData[0] = data;

    DL_I2C_setTargetAddress(I2C_INST, LCD_I2C_ADDRESS);

    DL_I2C_fillControllerTXFIFO(I2C_INST, txData, 1);

    DL_I2C_startControllerTransfer(I2C_INST, LCD_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY) {
        // Wait for the transfer to complete
    }
}

/* Function to create a millisecond delay */
void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for (i = 0; i < (ms * 1000); i++) {
        __asm("NOP");
    }
}

/* Temperature sensing function */
int temperature(void)
{
    uint32_t adcResult;
    _iq15 qVSample, qTsample, qTempDegF, qVTrim;

    SYSCFG_DL_init();

    qVTrim = _IQ15div(_IQ15mpy((_IQ15(DL_SYSCTL_getTempCalibrationConstant()) -
            _IQ15(0.5)), _IQ15(ADC_VREF_VOLTAGE)), ((uint32_t)(1) << 27));

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    gCheckADC = false;
    while (1) {
        DL_ADC12_startConversion(ADC12_0_INST);

        while (false == gCheckADC) {
            __WFE();
        }
        gCheckADC = false;

        DL_ADC12_stopConversion(ADC12_0_INST);

        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);

        qVSample = _IQ15div(_IQ15mpy((adcResult << 15) - _IQ15(0.5), _IQ15(ADC_VREF_VOLTAGE)), _IQ15(ADC_BIT_RESOLUTION));

        qTsample = _IQ15mpy(_IQ15(TEMP_TS_COEF_mV_C), (qVSample - qVTrim)) + (TEMP_TS_TRIM_C << 15);

        qTempDegF = _IQ15mpy(qTsample , _IQ15(9.0/5.0)) + _IQ15(32.0);

        gTemperatureDegC = _IQ15toF(qTsample);
        gTemperatureDegF = _IQ15toF(qTempDegF);

        /* Display temperature on LCD */
        char buffer[16];

        // Display Celsius
        LCD_setCursor(1, 1);
        sprintf(buffer, "Temp C: %.2f", gTemperatureDegC);
        LCD_sendString(buffer);

        // Display Fahrenheit
        LCD_setCursor(2, 1);
        sprintf(buffer, "Temp F: %.2f", gTemperatureDegF);
        LCD_sendString(buffer);

        delay_ms(1000); // 1-second delay
    }
}

/* Interrupt handler for ADC12 */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
