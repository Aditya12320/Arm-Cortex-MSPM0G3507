#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdint.h>
#include "ti/driverlib/dl_i2c.h"

// Define ADC settings
#define ADC12_BIT_RESOLUTION          (12)
#define ADC12_EXTERNAL_REF_VOLTAGE    (5.0)

// I2C LCD settings
#define LCD_I2C_ADDRESS 0x27
#define LCD_LINE_1 0x80
#define LCD_LINE_2 0xC0

// Flag for ADC conversion completion
volatile bool gCheckADC = false;

// Function prototypes
void I2C_init(void);
void LCD_init(void);
void LCD_sendCommand(uint8_t cmd);
void LCD_sendData(uint8_t data);
void LCD_sendString(char *str);
void LCD_setCursor(uint8_t row, uint8_t col);
void delay_ms(uint32_t ms);
void LCD_sendByte(uint8_t byte, uint8_t mode);
void I2C_writeByte(uint8_t data);
void read_ADC_and_display_voltage(void);
void float_to_string(float num, char* buffer);

// Main function
int main(void)
{
    SYSCFG_DL_init();     // Initialize system configuration

    // Initialize I2C and LCD
    I2C_init();
    LCD_init();

    // Enable ADC interrupt for ADC0 (Potentiometer)
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    // Main loop
    while (1) {
        read_ADC_and_display_voltage();
        delay_ms(50);  // Delay to control refresh rate
    }
}

// ADC reading and display function
void read_ADC_and_display_voltage(void) {
    uint16_t adcResult;
    float voltage;
    char voltageStr[16];  // Buffer to store voltage as a string

    // Reset the ADC flag for the next conversion
    gCheckADC = false;

    // Start ADC conversion
    DL_ADC12_startConversion(ADC12_0_INST);

    while (false == gCheckADC) {
        __WFE();
    }

    // Get ADC result
    adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

    // Convert ADC result to voltage
    voltage = (adcResult * ADC12_EXTERNAL_REF_VOLTAGE) / (1 << ADC12_BIT_RESOLUTION);

    // Convert the voltage value to a string manually (replace sprintf)
    float_to_string(voltage, voltageStr);

    // Clear and update LCD display
    LCD_setCursor(1, 1);            // Set cursor to line 1
    LCD_sendString(voltageStr);      // Display voltage value

    // Stop ADC conversion
    DL_ADC12_stopConversion(ADC12_0_INST);
    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);

    // Debugging: Print raw ADC result and calculated voltage
    printf("Raw ADC Result: %u, Voltage: %.2fV\n", adcResult, voltage);
}

// Function to convert float to string (manual implementation)
void float_to_string(float num, char* buffer) {
    int whole = (int) num;
    int frac = (int)((num - whole) * 100);  // Get two decimal places
    sprintf(buffer, "Voltage: %d.%02dV", whole, frac);
}

// ADC interrupt handler
void ADC12_0_INST_IRQHandler(void)
{
    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST) == DL_ADC12_IIDX_MEM0_RESULT_LOADED) {
        gCheckADC = true;
        // Clear the interrupt flag after processing
//        DL_ADC12_clearPendingInterrupt(ADC12_0_INST);
    }
}

// Initialize I2C
void I2C_init(void)
{
    DL_I2C_enableController(I2C_INST);  // Initialize I2C controller
}

// Initialize LCD in 4-bit mode
void LCD_init(void)
{
    delay_ms(50);
    LCD_sendCommand(0x33);  // Initialize to 8-bit mode
    LCD_sendCommand(0x32);  // Switch to 4-bit mode
    LCD_sendCommand(0x28);  // Function set: 4-bit, 2 lines
    LCD_sendCommand(0x0C);  // Display ON, cursor OFF
    LCD_sendCommand(0x06);  // Entry mode set: Increment cursor
    LCD_sendCommand(0x01);  // Clear display
    delay_ms(2);
}

// Send command to LCD
void LCD_sendCommand(uint8_t cmd)
{
    uint8_t upper_nibble = cmd & 0xF0;
    uint8_t lower_nibble = (cmd << 4) & 0xF0;

    LCD_sendByte(upper_nibble, 0);
    delay_ms(2);
    LCD_sendByte(lower_nibble, 0);
    delay_ms(2);
}

// Send data to LCD
void LCD_sendData(uint8_t data)
{
    uint8_t upper_nibble = data & 0xF0;
    uint8_t lower_nibble = (data << 4) & 0xF0;

    LCD_sendByte(upper_nibble, 1);
    LCD_sendByte(lower_nibble, 1);
}

// Send byte to LCD with mode (command/data)
void LCD_sendByte(uint8_t byte, uint8_t mode)
{
    uint8_t data = byte | (mode ? 0x01 : 0x00);  // RS bit for command/data
    data |= 0x08;  // Enable backlight

    I2C_writeByte(data | 0x04);  // Set EN high
    delay_ms(1);
    I2C_writeByte(data & ~0x04);  // Set EN low
    delay_ms(1);
}

// Set cursor to specific row and column
void LCD_setCursor(uint8_t row, uint8_t col)
{
    uint8_t address;
    switch (row)
    {
        case 1: address = LCD_LINE_1 + (col - 1); break;
        case 2: address = LCD_LINE_2 + (col - 1); break;
        default: address = LCD_LINE_1; break;
    }
    LCD_sendCommand(address);
}

// Send string to LCD
void LCD_sendString(char *str)
{
    while (*str) {
        LCD_sendData(*str++);
    }
}

// Write byte to I2C bus
void I2C_writeByte(uint8_t data)
{
    uint8_t txData[1] = { data };

    DL_I2C_setTargetAddress(I2C_INST, LCD_I2C_ADDRESS);
    DL_I2C_fillControllerTXFIFO(I2C_INST, txData, 1);
    DL_I2C_startControllerTransfer(I2C_INST, LCD_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY) {
        // Wait for transfer to complete
    }
}

// Delay function
void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for (i = 0; i < (ms * 1000); i++) {
        __asm("NOP");
    }
}
