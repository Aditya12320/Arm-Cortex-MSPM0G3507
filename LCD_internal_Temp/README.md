# Temperature Display with I2C LCD Using MSPM0G3507

This project demonstrates how to read temperature data from a sensor using the ADC (Analog-to-Digital Converter) of the MSPM0G3507 microcontroller and display the temperature in both Celsius and Fahrenheit on an I2C LCD. The code is written in C using Texas Instruments' DriverLib and IQMathLib for floating-point calculations.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Wiring Diagram](#wiring-diagram)
- [Code Structure](#code-structure)
- [How to Run](#how-to-run)
- [License](#license)

## Overview
The project initializes an I2C LCD display and an ADC to read the temperature from a sensor. The ADC result is converted into temperature values, which are displayed on the LCD screen in both Celsius and Fahrenheit. The temperature sensing logic is enhanced using TI's IQMathLib for fixed-point arithmetic to handle precision.

## Features
- Reads analog temperature sensor values.
- Converts ADC readings into Celsius and Fahrenheit.
- Displays temperature on a 16x2 I2C LCD in real-time.
- Uses MSPM0G3507 microcontroller with ADC and I2C.

## Hardware Requirements
- **MSPM0G3507 LaunchPad** (or compatible)
- **16x2 I2C LCD Display** (Address: 0x27 or 0x3F)
- **Temperature Sensor** (internal ADC sensor or external sensor)
- **Connecting Wires**

## Software Requirements
- **Code Composer Studio** or any compatible IDE
- **Texas Instruments DriverLib**
- **IQMathLib for Fixed-Point Arithmetic**
- **SYSCFG_DL Initialization Libraries**

## Wiring Diagram

| MSPM0G3507 Pins | I2C LCD Pins |
|-----------------|--------------|
| I2C SDA         | SDA          |
| I2C SCL         | SCL          |
| VCC             | VCC          |
| GND             | GND          |

## Code Structure
- **`main.c`**: Contains the main function and the logic for initializing the I2C, ADC, and LCD, as well as continuously reading and displaying temperature values.
- **`LCD_init`**: Initializes the LCD in 4-bit mode.
- **`temperature`**: Reads ADC values, converts them into temperature, and updates the LCD display.
- **`I2C_writeByte`**: Sends data over the I2C bus.
- **`LCD_setCursor`**: Sets the cursor on the LCD to display data at a specified row and column.

## How to Run
1. **Hardware Setup**: 
   - Connect the I2C LCD and temperature sensor to the MSPM0G3507 LaunchPad as per the wiring diagram.
2. **Compile and Upload**: 
   - Use Code Composer Studio (or other compatible IDE) to compile the code and upload it to the MSPM0G3507 board.
3. **Run**: 
   - After uploading, the LCD should start displaying the temperature values in both Celsius and Fahrenheit.
## OUTPUT
![Temperature](<Screenshot 2024-10-07 153107.png>)

## License
This project is open-source and available under the MIT License.
