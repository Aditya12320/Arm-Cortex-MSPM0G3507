#include "ti_msp_dl_config.h"
#include <stdio.h>  // Include this to use printf

/*
 * Configuration to check ADC conversion if below or above
 * (external reference voltage / 2)
 *
 * Important: Update ADC12_EXTERNAL_REF_VOLTAGE with actual external reference
 * voltage
 */

/* clang-format off */
#define ADC12_BIT_RESOLUTION          (12)
#define ADC12_EXTERNAL_REF_VOLTAGE    (5.0)
#define ADC12_MONITOR_VOLTAGE         (ADC12_EXTERNAL_REF_VOLTAGE / 2)
#define ADC12_MONITOR_VALUE           ((1 << ADC12_BIT_RESOLUTION) * (ADC12_MONITOR_VOLTAGE / (ADC12_EXTERNAL_REF_VOLTAGE)))
/* clang-format on */

volatile bool gCheckADC = false;  // Flag to check ADC conversion

int main(void)
{
    uint16_t adcResult;
    float voltage;

    SYSCFG_DL_init();  // Initialize system configuration

    // Enable ADC interrupt for ADC0
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);  // Enable ADC0 interrupt

    // Initialize flag for ADC channel
    gCheckADC = false;

    while (1) {
        // Start ADC conversion for the channel
        DL_ADC12_startConversion(ADC12_0_INST);  // Start ADC0 conversion (Potentiometer)

        // Wait until the ADC conversion is done
        while (!gCheckADC) {
            __WFE();  // Wait for event (conversion complete)
        }

        // Get ADC result from the channel
        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        voltage = (adcResult * ADC12_EXTERNAL_REF_VOLTAGE) / (1 << ADC12_BIT_RESOLUTION);  // Convert ADC result to voltage

        // Print the ADC value and corresponding voltage for the potentiometer
        printf("Potentiometer - ADC Value: %d, Voltage: %.2f V\n", adcResult, voltage);

        // Stop ADC conversion for the channel
        DL_ADC12_stopConversion(ADC12_0_INST);
        gCheckADC = false;  // Reset the flag for the next conversion
    }
}

// ADC interrupt handler for ADC0 (Potentiometer)
void ADC12_0_INST_IRQHandler(void)
{
    // Check if the interrupt is for memory 0
    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST) == DL_ADC12_IIDX_MEM0_RESULT_LOADED) {
        gCheckADC = true;  // Set flag for ADC0 conversion complete
    }
}
