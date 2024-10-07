#include <ti/iqmath/include/IQmathLib.h>
#include "ti_msp_dl_config.h"
#include <stdio.h> // Added for printing to the console

/* clang-format off */

/*
 * The following trim parameter are provided in the device datasheet in chapter
 * "Temperature Sensor"
 */
#define TEMP_TS_TRIM_C                                            ((uint32_t)30)
/*
 * Constant below is (1/TSc). Where TSc is Temperature Sensor coefficient
 * available in the device datasheet
 */
#define TEMP_TS_COEF_mV_C                                             (-555.55f)

#define ADC_VREF_VOLTAGE                                                  (3.3f)
#define ADC_BIT_RESOLUTION                                   ((uint32_t)(1)<<12)

/* clang-format off */

volatile bool gCheckADC;
volatile float gTemperatureDegC;
volatile float gTemperatureDegF;

int temperature(void)
{
    uint32_t adcResult;
    _iq15 qVSample, qTsample, qTempDegF, qVTrim;

    SYSCFG_DL_init();

    /*
     * Convert TEMP_SENSE0 result to equivalent voltage:
     * Vtrim = (ADC_VREF_VOLTAGE*(TEMP_SENSE0 -0.5))/(2^12)
     */
    qVTrim = _IQ15div(_IQ15mpy((_IQ15(DL_SYSCTL_getTempCalibrationConstant()) -
            _IQ15(0.5)), _IQ15(ADC_VREF_VOLTAGE)), ((uint32_t)(1) << 27));

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    gCheckADC = false;
    while (1) {

        DL_ADC12_startConversion(ADC12_0_INST);  // Start ADC conversion

        while (false == gCheckADC) {
            __WFE();  // Wait for event (interrupt)
        }
        gCheckADC = false;  // Reset flag after the conversion is done

        DL_ADC12_stopConversion(ADC12_0_INST);   // Stop ADC conversion

        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);

        /*
         * Convert ADC result to equivalent voltage:
         * Vsample = (VREF_VOLTAGE_MV*(adcResult -0.5))/(2^ADC_BIT_RESOLUTION)
         */
        qVSample = _IQ15div(_IQ15mpy((adcResult << 15) -
                _IQ15(0.5), _IQ15(ADC_VREF_VOLTAGE)), _IQ15(ADC_BIT_RESOLUTION));

        /*
         * Apply temperature sensor calibration data
         * TSAMPLE = (TEMP_TS_COEF_mV_C) * (qAdcResultV - vTrim) + TEMP_TS_TRIM_C
         */
        qTsample = _IQ15mpy(_IQ15(TEMP_TS_COEF_mV_C), (qVSample - qVTrim)) +
                            (TEMP_TS_TRIM_C << 15);

        qTempDegF = _IQ15mpy(qTsample , _IQ15(9.0/5.0)) + _IQ15(32.0);

        // Convert IQ format values to floating-point for printing
        gTemperatureDegC = _IQ15toF(qTsample);
        gTemperatureDegF = _IQ15toF(qTempDegF);

        // Print temperature values to the console
        printf("Temperature in Celsius: %f\n", gTemperatureDegC);
        printf("Temperature in Fahrenheit: %f\n", gTemperatureDegF);

        // Add a small delay (e.g., 1 second) before the next conversion
        for (volatile int i = 0; i < 1000000; i++) {
            __NOP();  // Simple delay loop
        }
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
