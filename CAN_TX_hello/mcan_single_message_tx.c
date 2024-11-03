#include "ti_msp_dl_config.h"
#include <stdio.h>  // For printf

#define HELLO_MESSAGE "Hello"  // The message we want to send
#define HELLO_MSG_LENGTH 5     // Length of the message "Hello"

volatile bool gTXMsg;
volatile bool error;

int main(void)
{
    DL_MCAN_TxBufElement txMsg;

    SYSCFG_DL_init();

    /* Initialize the message to transmit. */
    /* Identifier Value. */
    txMsg.id = ((uint32_t)(0x4)) << 18U;
    /* Transmit data frame. */
    txMsg.rtr = 0U;
    /* 11-bit standard identifier. */
    txMsg.xtd = 0U;
    /* ESI bit in CAN FD format depends only on error passive flag. */
    txMsg.esi = 0U;
    /* Transmitting 5 bytes ("Hello"). */
    txMsg.dlc = HELLO_MSG_LENGTH;
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg.brs = 1U;
    /* Frame transmitted in CAN FD format. */
    txMsg.fdf = 1U;
    /* Store Tx events. */
    txMsg.efc = 1U;
    /* Message Marker. */
    txMsg.mm = 0xAAU;

    // Copy the "Hello" message into the data array
    txMsg.data[0] = 'H';
    txMsg.data[1] = 'e';
    txMsg.data[2] = 'l';
    txMsg.data[3] = 'l';
    txMsg.data[4] = 'o';

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    // Wait for CAN to be in normal operation mode
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    while (1) {
        gTXMsg = false;

        /* Wait until button is pressed to send the message */
        while (gTXMsg == false) {
            __WFE();  // Wait for Event (WFE) - low-power wait until an event occurs
        }

        // Transmit the message to CAN bus
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &txMsg);
        DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);

        // Print the transmitted message to the console
        printf("Transmitted message: %c%c%c%c%c\n",
            txMsg.data[0], txMsg.data[1], txMsg.data[2],
            txMsg.data[3], txMsg.data[4]);

        // Reset the flag to allow retransmission
        gTXMsg = false;
    }
}

/* Interrupt handler for button press to trigger CAN message transmission */
void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case DL_GPIO_IIDX_DIO21:
                    gTXMsg = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
