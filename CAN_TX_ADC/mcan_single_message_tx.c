#include "ti_msp_dl_config.h"
#include <stdio.h>

#define ADC12_BIT_RESOLUTION          (12)
#define ADC12_EXTERNAL_REF_VOLTAGE    (5.0)

volatile bool gCheckADC = false;  // Flag to check ADC conversion

int main(void)
{
    uint16_t adcResult;
    float voltage;
    DL_MCAN_TxBufElement txMsg;  // CAN message structure

    SYSCFG_DL_init();  // Initialize system configuration

    // Enable ADC interrupt for ADC0
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    // Initialize flag for ADC channel
    gCheckADC = false;

    // Configure the CAN message structure
    txMsg.id = ((uint32_t)(0x4)) << 18U;
    txMsg.rtr = 0U;  // Data frame
    txMsg.xtd = 0U;  // Standard identifier
    txMsg.dlc = 4U;  // Sending 4 bytes for the float value

    while (1) {
        // Start ADC conversion
        DL_ADC12_startConversion(ADC12_0_INST);

        while (false == gCheckADC) {
            __WFE();  // Wait for ADC conversion
        }

        // Get the ADC result
        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

        // Convert ADC result to voltage
        voltage = (adcResult * ADC12_EXTERNAL_REF_VOLTAGE) / (1 << ADC12_BIT_RESOLUTION);

        // Prepare CAN message with voltage value
        uint32_t voltageAsInt = *(uint32_t*)&voltage;  // Interpret float as uint32_t for transmission
        txMsg.data[0] = (voltageAsInt >> 24) & 0xFF;  // Highest byte
        txMsg.data[1] = (voltageAsInt >> 16) & 0xFF;  // Second highest byte
        txMsg.data[2] = (voltageAsInt >> 8) & 0xFF;   // Second lowest byte
        txMsg.data[3] = voltageAsInt & 0xFF;          // Lowest byte

        // Write message to CAN bus
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &txMsg);
        DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);  // Request CAN transmission

        // Print the transmitted voltage to the console
        printf("Transmitted Voltage: %.2f V\n", voltage);

        // Stop ADC conversion and reset the flag
        DL_ADC12_stopConversion(ADC12_0_INST);
        gCheckADC = false;
        DL_ADC12_enableConversions(ADC12_0_INST);
        delay_cycles(1600000);
    }
}

// ADC interrupt handler
void ADC12_0_INST_IRQHandler(void)
{
    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST) == DL_ADC12_IIDX_MEM0_RESULT_LOADED) {
        gCheckADC = true;
    }
}
