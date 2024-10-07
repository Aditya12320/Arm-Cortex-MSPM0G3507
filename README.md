# ARM Cortex-M0 MSPM0G3506 Microcontroller Project

This project demonstrates various functionalities of the ARM Cortex-M0 MSPM0G3506 microcontroller using Code Composer Studio. The project includes ADC readings, I2C communication, and LCD display integration.

## Project Components

1. ADC External Potentiometer
   - Location: `ADC_External_POT` folder
   - Description: Reads values from an external potentiometer using ADC and prints them to the console.

2. ADC Internal Temperature Sensor
   - Location: `ADC_internal_Temp` folder
   - Description: Reads values from the internal temperature sensor using ADC and prints them to the console.

3. I2C LCD Interface
   - Location: `I2C_LCD` folder
   - Description: Implements I2C communication protocol to interface with an LCD.

4. LCD Display of Internal Temperature
   - Location: `LCD_internal_Temp` folder
   - Description: Reads internal temperature values and displays them on the LCD using I2C.

5. LCD Display of Potentiometer Values
   - Location: `LCD_external_POT` folder
   - Description: Reads external potentiometer values and displays them on the LCD using I2C.

## Setup and Installation

1. Clone this repository to your local machine.
2. Open Code Composer Studio.
3. Import the project folders into your workspace.
4. Ensure that the MSPM0G3506 microcontroller is properly connected to your development setup.
5. Configure Code Composer Studio for the MSPM0G3506 target.

## Usage

Each subfolder contains a separate project demonstrating a specific functionality. To run a particular project:

1. Open the desired project in Code Composer Studio.
2. Build the project to compile the code.
3. Load the compiled code onto the MSPM0G3506 microcontroller.
4. Run the program and observe the output on either the console or the LCD, depending on the project.
