# Temperature Measurement Project

## Overview

This project demonstrates temperature measurement using the MSPM0G3507 microcontroller, an ADC module, and the IQMath library for accurate mathematical operations. The temperature readings are converted to both Celsius and Fahrenheit, displayed in the console.

## Components

- **MSPM0G3507 Launchpad**
- **Temperature Sensor** (internal/external)
- **Code Composer Studio** or any compatible IDE

## Functionality

The main function continuously measures the temperature by reading from the ADC and converting the raw values into temperature using calibration data. The calculated temperature is printed to the console in both Celsius and Fahrenheit.

### Main Function: `temperature()`

- **Initialization**: Sets up the ADC and enables interrupts.
- **ADC Conversion**: Starts the ADC conversion, waits for the result, and stops the conversion.
- **Voltage Calculation**: Converts the ADC result to voltage.
- **Temperature Calculation**: Uses calibration data to compute the temperature in Celsius and Fahrenheit.
- **Console Output**: Prints the temperature values to the console.
- **Delay**: Introduces a delay before the next measurement.

## Usage

1. Connect the temperature sensor to the MSPM0G3507 Launchpad.
2. Compile and upload the code using Code Composer Studio.
3. Open a console to view the temperature readings in real-time.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
