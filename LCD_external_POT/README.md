# MSPM0G3507 LaunchPad ADC to I2C LCD Display

## Project Overview
This project demonstrates how to read an analog input using the ADC on the Texas Instruments MSPM0G3507 LaunchPad and display the converted voltage on an I2C-based LCD screen. It utilizes the MSP DriverLib for handling ADC and I2C communications.

## Components Used:
- **MSPM0G3507 LaunchPad**: Microcontroller board from Texas Instruments.
- **I2C LCD (16x2)**: A 16x2 character display with an I2C module for communication.
- **Potentiometer**: Used to simulate an analog voltage input to the ADC.
- **TI DriverLib**: Provides simplified APIs for peripheral handling on the MSPM0G3507.

## Functionality:
1. **ADC**: Reads the analog voltage from a potentiometer or any analog source connected to the ADC input pin.
2. **I2C LCD Display**: Displays the corresponding voltage value after conversion from the ADC.

### Key Features:
- 12-bit resolution ADC.
- I2C communication with the LCD.
- Real-time voltage display.
  
## Getting Started:

### Prerequisites:
- **MSPM0G3507 LaunchPad** development kit.
- I2C-based 16x2 LCD display.
- Potentiometer (or similar analog input).
- TI Code Composer Studio (CCS) or any compatible IDE for building and flashing the code.

### Software Libraries:
- **MSP DriverLib**: This project uses Texas Instruments' MSP DriverLib to handle peripherals like ADC and I2C.

### Hardware Connections:
1. **Potentiometer (Analog Input)**:
   - Connect the middle pin of the potentiometer to the ADC input pin (Pin X of MSPM0G3507).
   - Connect the other two pins to VCC and GND.
   
2. **I2C LCD**:
   - Connect the I2C pins (SDA, SCL) of the LCD to the corresponding I2C pins on the MSPM0G3507 (typically P6.4 for SCL and P6.5 for SDA).
   - Connect VCC and GND of the LCD to the respective power lines on the MSPM0G3507.

### Installation and Usage:

1. **Clone the repository**:
    ```bash
    git clone https://github.com/your-repository/adc-i2c-lcd.git
    ```

2. **Build and Flash**:
   - Open the project in Code Composer Studio or another compatible IDE.
   - Build the project and flash the code onto the MSPM0G3507 LaunchPad.

3. **Run**:
   - Once the code is uploaded, the LaunchPad will read the analog voltage from the potentiometer and display the corresponding voltage value on the I2C LCD.

### Code Overview:
- **ADC12 Configuration**: The ADC is configured to read analog input and trigger an interrupt when the conversion is complete.
- **I2C LCD**: A simple driver is implemented for sending commands and data to the LCD over I2C.
- **Voltage Calculation**: The ADC value is converted to a voltage using a scaling formula and displayed on the LCD.

### Example Output:




### Future Improvements:
- Add multiple ADC channels for monitoring different sensors.
- Implement more advanced LCD functionalities (scrolling text, custom characters, etc.).
  
## License:
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author:
- Aditya Jilla
