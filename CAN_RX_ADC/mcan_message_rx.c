#include "ti_msp_dl_config.h"
#include<stdio.h>
#include <stdint.h>
#include "ti/driverlib/dl_i2c.h"

// LCD initialization
#define LCD_I2C_ADDRESS 0x27

/* LCD row addresses */
#define LCD_LINE_1 0x80  // Address of the first character on line 1
#define LCD_LINE_2 0xC0  // Address of the first character on line 2
#define LCD_LINE_3 0x94  // Address of the first character on line 3
#define LCD_LINE_4 0xD4  // Address of the first character on line 4

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
void LCD_clear(void);

#define LED_STATUS_ON ((uint8_t) 0x00)
#define LED_STATUS_OFF ((uint8_t) 0x01)

#define ID_MODE_STANDARD (0x0U)
#define ID_MODE_EXTENDED (0x1U)

volatile uint32_t gInterruptLine1Status;
volatile bool gServiceInt;

static void processRxMsg(DL_MCAN_RxBufElement *rxMsg);
float voltage;
int main(void)
{
    DL_MCAN_RxBufElement rxMsg;
    DL_MCAN_RxFIFOStatus rxFS;

    SYSCFG_DL_init();

    I2C_init();

        /* Initialize LCD */
        LCD_init();


    gServiceInt           = false;
    gInterruptLine1Status = 0;

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    while (1) {
        /*
         * Wait for flag interrupt to be triggered and flag to check received
         * message is set to true.
         */
        while (false == gServiceInt) {
            __WFE();
        }
        gServiceInt  = false;
        rxFS.fillLvl = 0;

        if ((gInterruptLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
            rxFS.num = DL_MCAN_RX_FIFO_NUM_0;
            while ((rxFS.fillLvl) == 0) {
                DL_MCAN_getRxFIFOStatus(MCAN0_INST, &rxFS);
            }

            DL_MCAN_readMsgRam(
                MCAN0_INST, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &rxMsg);

            DL_MCAN_writeRxFIFOAck(MCAN0_INST, rxFS.num, rxFS.getIdx);

            processRxMsg(&rxMsg);

            gInterruptLine1Status &= ~(MCAN_IR_RF0N_MASK);
        }
    }
}

void processRxMsg(DL_MCAN_RxBufElement *rxMsg)
{
    uint32_t idMode;
    uint32_t id;

    idMode = rxMsg->xtd;

    if (ID_MODE_EXTENDED == idMode) {
        id = rxMsg->id;
    } else {
        /* Assuming package is using 11-bit standard ID.
         * When package uses standard id, ID is stored in ID[28:18]*/
        id = ((rxMsg->id & (uint32_t) 0x1FFC0000) >> (uint32_t) 18);
    }

    // Print received ID and first byte of data
//    printf("Received ID: 0x%X, Data[0]: 0x%X, Data[1]: 0x%X, Data[2]: 0x%X, Data[3]: 0x%X\n", id, rxMsg->data[0], rxMsg->data[1], rxMsg->data[2], rxMsg->data[3]);

    uint32_t voltageAsInt = (rxMsg->data[0] << 24) | (rxMsg->data[1] << 16) | (rxMsg->data[2] << 8) | rxMsg->data[3];
    voltage = *((float*)&voltageAsInt); // Assuming you're using a compatible float representation
    printf("Received Voltage: %.2f V\n", voltage);
                    LCD_clear();
                    LCD_setCursor(1, 1);
                    char buffer[16];  // Buffer to hold the string representation of voltage
                    sprintf(buffer, "%.2f", voltage); // Convert integer to string
                    LCD_sendString(buffer);
                    delay_ms(5);

    switch (id) {
        case 0x3:
            if (rxMsg->data[0] == LED_STATUS_ON) {
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            } else {
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            }
            break;
        case 0x4:
            if (rxMsg->data[0] == LED_STATUS_ON) {
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            } else {
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            }
            break;
        default:
            /* Don't do anything */
            break;
    }

}



void I2C_init(void)
{
    /* Initialize I2C with 100kHz speed */
    DL_I2C_enableController(I2C_INST); // Updated function name
}

/* Function to initialize the LCD in 4-bit mode */
void LCD_init(void)
{
    /* Initial delay for LCD power-up */
    delay_ms(50);

    /* Initialization sequence for 4-bit mode */
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

    /* Send high nibble */
    LCD_sendByte(upper_nibble, 0);
    delay_ms(2);   // Additional delay

    /* Send low nibble */
    LCD_sendByte(lower_nibble, 0);
    delay_ms(2);   // Additional delay
}

/* Function to send data to the LCD */
void LCD_sendData(uint8_t data)
{
    uint8_t upper_nibble = data & 0xF0;           // High nibble
    uint8_t lower_nibble = (data << 4) & 0xF0;    // Low nibble

    /* Send high nibble */
    LCD_sendByte(upper_nibble, 1);

    /* Send low nibble */
    LCD_sendByte(lower_nibble, 1);
}

/* Function to send a byte to the LCD with control bits */
void LCD_sendByte(uint8_t byte, uint8_t mode)
{
    uint8_t data;

    /* Mode: 0 = command, 1 = data */
    data = byte | (mode ? 0x01 : 0x00); // RS bit
    data |= 0x08;                       // Enable backlight

    /* Send byte with Enable bit high */
    I2C_writeByte(data | 0x04);         // En=1
    delay_ms(1);
    /* Send byte with Enable bit low */
    I2C_writeByte(data & ~0x04);        // En=0
    delay_ms(1);
}

/* Function to set the cursor to a specific row and column */
void LCD_setCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    /* Calculate the DDRAM address based on the row and column */
    switch (row)
    {
        case 1:
            address = LCD_LINE_1 + (col - 1);
            break;
        case 2:
            address = LCD_LINE_2 + (col - 1);
            break;
        case 3:
            address = LCD_LINE_3 + (col - 1);
            break;
        case 4:
            address = LCD_LINE_4 + (col - 1);
            break;
        default:
            address = LCD_LINE_1;
            break;
    }

    /* Send the command to set the DDRAM address */
    LCD_sendCommand(address);
}

/* Function to send a string to the LCD */
void LCD_sendString(char *str)
{
    while (*str) {
        LCD_sendData(*str++);           // Send each character of the string
    }
}

/* Function to write a byte to the I2C bus */
void I2C_writeByte(uint8_t data)
{
    uint8_t txData[1];
    txData[0] = data;

    /* Set the target address (7-bit unshifted address) */
    DL_I2C_setTargetAddress(I2C_INST, LCD_I2C_ADDRESS); // Updated function name

    /* Fill the TX FIFO with the data */
    DL_I2C_fillControllerTXFIFO(I2C_INST, txData, 1);

    /* Start the I2C transfer with correct direction */
    DL_I2C_startControllerTransfer(I2C_INST, LCD_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 1); // Changed direction constant to TX

    /* Wait for transfer to complete */
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY) {
        // Wait for the transfer to complete
    }
}

/* Function to create a millisecond delay */
void delay_ms(uint32_t ms)
{
    // Adjust this value based on your microcontroller's clock speed
    volatile uint32_t i;
    for (i = 0; i < (ms * 1000); i++) {
        __asm("NOP");
    }
}
void LCD_clear(void)
{
    LCD_sendCommand(0x01);  // Send the clear display command
    delay_ms(2);             // Wait for the command to be processed
}
void MCAN0_INST_IRQHandler(void)
{
    switch (DL_MCAN_getPendingInterrupt(MCAN0_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status |= DL_MCAN_getIntrStatus(MCAN0_INST);
            DL_MCAN_clearIntrStatus(MCAN0_INST, gInterruptLine1Status,
                DL_MCAN_INTR_SRC_MCAN_LINE_1);

            gServiceInt = true;
            break;
        default:
            break;
    }
}
