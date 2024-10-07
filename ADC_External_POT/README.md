# Potentiometer ADC Measurement

This project demonstrates how to read the value of a potentiometer connected to an ADC channel of the MSPM0G3507 microcontroller using the TI MSP Driver Library.

## Features

- **Single ADC Channel**: Measures the value from a single potentiometer.
- **Interrupt-Driven**: Uses ADC interrupts to handle conversion completion.
- **Voltage Calculation**: Converts the ADC value to a corresponding voltage based on a defined external reference voltage.

## Hardware Requirements

- **MSPM0G3507 LaunchPad**: Texas Instruments microcontroller development kit.
- **Potentiometer**: Connect a potentiometer to the ADC channel.
- **External Reference Voltage**: Set the reference voltage according to your circuit design.

## Code Explanation

### Key Functions

1. **SYSCFG_DL_init()**
   - Initializes the system configuration for the microcontroller.

2. **DL_ADC12_startConversion()**
   - Starts the ADC conversion for the specified channel (ADC0 in this case).

3. **DL_ADC12_getMemResult()**
   - Retrieves the ADC conversion result from memory.

4. **DL_ADC12_stopConversion()**
   - Stops the ADC conversion process for the specified channel.

5. **NVIC_EnableIRQ()**
   - Enables the interrupt for ADC conversion completion.

### ADC Interrupt Handler

- **ADC12_0_INST_IRQHandler()**
   - Triggered when the ADC conversion is complete. Sets a flag (`gCheckADC`) to indicate the conversion result is ready.

## How to Run

1. Connect a potentiometer to the ADC channel on the MSPM0G3507.
2. Configure the reference voltage in the code.
3. Compile and upload the code to the microcontroller.
4. Open a serial terminal to view the ADC values and corresponding voltage readings.

## License

This project is licensed under the MIT License.
