/*
 * Include necessary header files
 */
#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdint.h>
#include "ti/driverlib/dl_i2c.h"

/* I2C LCD address (commonly 0x27 or 0x3F for I2C LCD modules) */
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

/* Main function */
int main(void)
{
    SYSCFG_DL_init();

    /* Initialize I2C module */
    I2C_init();

    /* Initialize LCD */
    LCD_init();

    /* Set cursor to row 1, column 5 and display "MSP" */
    LCD_setCursor(1, 1);
    LCD_sendString("Hello");

    /* Set cursor to row 2, column 1 and display more text */
    LCD_setCursor(2, 1);
    LCD_sendString("LCD Is working!");

    delay_ms(500000);

    while (1) {
        /* Infinite loop */
    }
}

/* Initialize the I2C module */
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
