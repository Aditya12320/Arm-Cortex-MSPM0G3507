# I2C LCD Interface with MSPM0G3507

This project demonstrates interfacing a 20x4 I2C LCD display with the Texas Instruments MSPM0G3507 microcontroller using I2C communication. The LCD is controlled using the DL I2C driver provided in the `ti_msp_dl_config.h` library.

## Features
- Initializes the I2C module and the LCD.
- Displays text on the LCD at specified rows and columns.
- Supports custom delays and character positioning on the display.

## Components Used
1. **MSPM0G3507 Launchpad Development Kit** - A powerful ARM Cortex-M0+ microcontroller from Texas Instruments.
2. **I2C LCD Module (20x4)** - A common 20x4 character LCD module with an I2C interface.
3. **TI Driver Library (ti/driverlib/dl_i2c)** - For enabling and managing I2C communication.

## Prerequisites
- **MSPM0G3507 Development Environment**: You need to set up the development environment for the MSPM0G3507, including Code Composer Studio (CCS) or your preferred IDE.
- **Hardware**:
  - MSPM0G3507 Launchpad
  - 20x4 I2C LCD module
  - Jumper wires for connections

## Wiring
Below are the updated pin connections for interfacing the LCD with the MSPM0G3507 using I2C:

| LCD Pin       | MSPM0G3507 Pin | Description         |
| ------------- | -------------- | ------------------- |
| SDA (I2C Data) | PA3            | I2C Data (SDA)      |
| SCL (I2C Clock)| PA2            | I2C Clock (SCL)     |
| VCC           | 3.3V           | Power Supply        |
| GND           | GND            | Ground              |

## How It Works
The code initializes the I2C interface and sends the necessary commands to set up the LCD in 4-bit mode. Text is displayed on the LCD by setting the cursor to a specific row and column and sending strings to be shown on the screen.

### Functions
- `I2C_init()`: Initializes the I2C interface with the MSPM0G3507.
- `LCD_init()`: Sends commands to initialize the LCD in 4-bit mode.
- `LCD_setCursor(row, col)`: Sets the cursor to a specific row and column.
- `LCD_sendString()`: Sends a string of characters to be displayed on the LCD.
- `delay_ms(ms)`: Custom millisecond delay function.

### Example
In the `main` function, the text "Hello" is displayed on the first line of the LCD, and "LCD IS working!" is displayed on the second line:

```c
/* Set cursor to row 1, column 1 and display "Swapnil" */
LCD_setCursor(1, 1);
LCD_sendString("Hello");

/* Set cursor to row 2, column 1 and display "Aditya" */
LCD_setCursor(2, 1);
LCD_sendString("LCD is Working!");
