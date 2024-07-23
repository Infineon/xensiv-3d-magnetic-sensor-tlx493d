// std includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

// XMC includes
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_spi.h"
#include "xmc_ccu4.h"
#include "xmc_common.h"

// project c includes
#include "types.h"
#include "tlx493d_types.h"
#include "BoardSupportUsingKit2Go.h"
#include "Logger.h"

#include "TLx493D_A1B6.h"
#include "TLx493D_A2B6.h"
#include "TLx493D_P2B6.h"
#include "TLx493D_W2B6.h"
#include "TLx493D_W2BW.h"
#include "TLx493D_P3B6.h"


#include "read_iic_sensor_with_wakeup.h"

//   Here a XMC4700 Relax Kit is used as microcontroller.

// POWER PIN - arduino pin 8 and INTERRUPT PIN INT0 for XMC4700 Relax Kit
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#define TLx493D_INTERRUPT_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 0   
#define TLx493D_INTERRUPT_CCU4_MODULE CCU40
#define TLx493D_INTERRUPT_CCU4_SLICE CCU40_CC43
#define TLx493D_INTERRUPT_CCU4_SLICE_NUM 3
#define TLx493D_INTERRUPT_SLICE_INPUT CCU40_IN3_P1_0 
#define TLx493D_INTERRUPT_IRQ_MULTI_EVENT_ID XMC_CCU4_SLICE_MULTI_IRQ_ID_EVENT0
#define TLx493D_INTERRUPT_IRQ_EVENT_ID XMC_CCU4_SLICE_IRQ_ID_EVENT0
#define TLx493D_INTERRUPT_IRQ_EVENT_ID_NUM 0
#define TLx493D_INTERRUPT_SLICE_EVENT_ID XMC_CCU4_SLICE_EVENT_0
#define TLx493D_INTERRUPT_CCU4_IRQ CCU40_0_IRQn
#define TLx493D_INTERRUPT_CCU4_IRQ_HDLR CCU40_0_IRQHandler
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001) //INT1 
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#define TLx493D_INTERRUPT_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 0
#define TLx493D_INTERRUPT_CCU4_MODULE CCU40
#define TLx493D_INTERRUPT_CCU4_SLICE CCU40_CC40
#define TLx493D_INTERRUPT_CCU4_SLICE_NUM 0
#define TLx493D_INTERRUPT_SLICE_INPUT CCU40_IN0_U0C0_DX2INS
#define TLx493D_INTERRUPT_IRQ_MULTI_EVENT_ID XMC_CCU4_SLICE_MULTI_IRQ_ID_EVENT1
#define TLx493D_INTERRUPT_IRQ_EVENT_ID_NUM 1
#define TLx493D_INTERRUPT_IRQ_EVENT_ID XMC_CCU4_SLICE_IRQ_ID_EVENT1
#define TLx493D_INTERRUPT_SLICE_EVENT_ID XMC_CCU4_SLICE_EVENT_1
#define TLx493D_INTERRUPT_CCU4_IRQ CCU40_1_IRQn
#define TLx493D_INTERRUPT_CCU4_IRQ_HDLR CCU40_1_IRQHandler
#endif

 /** Definition of the upper and lower thresholds in X, Y, Z-direction.
 *  These thresholds define under which conditions the sensor wakes up and triggers his interrupt.
 */
const int16_t THRESHOLD = 100;

const int16_t LOWER_THRESHOLD_X = -THRESHOLD;
const int16_t LOWER_THRESHOLD_Y = -THRESHOLD;
const int16_t LOWER_THRESHOLD_Z = -THRESHOLD;

const int16_t UPPER_THRESHOLD_X = THRESHOLD;
const int16_t UPPER_THRESHOLD_Y = THRESHOLD;
const int16_t UPPER_THRESHOLD_Z = THRESHOLD;

bool intTriggered = false;


void read_iic_sensor_with_wakeup(){
   

    // powering up the board
    static Kit2GoBoardSupport k2go;
    bsc_initAttributes(&k2go);
    bsc_setPowerPin(&k2go, TLx493D_POWER_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000); //arduino pin 8
    
    bsc_init(&k2go, true, false, false);

    printf("Power-up done\n");

    // sensor object
    TLx493D_t dut;

    /** Definition of the interrupt pin and logic **/

    const XMC_GPIO_CONFIG_t gpio_config = {.mode = XMC_GPIO_MODE_INPUT_TRISTATE, .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW};
    XMC_GPIO_Init(TLx493D_INTERRUPT_PIN_PORT_AND_NUM, &gpio_config);

    const XMC_CCU4_SLICE_EVENT_CONFIG_t event_config = {  .edge = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE, .mapped_input = TLx493D_INTERRUPT_SLICE_INPUT };

    XMC_CCU4_Init(TLx493D_INTERRUPT_CCU4_MODULE, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
    XMC_CCU4_EnableClock(TLx493D_INTERRUPT_CCU4_MODULE, TLx493D_INTERRUPT_CCU4_SLICE_NUM);
    XMC_CCU4_SLICE_EnableMultipleEvents(TLx493D_INTERRUPT_CCU4_SLICE, TLx493D_INTERRUPT_IRQ_MULTI_EVENT_ID);
    XMC_CCU4_SLICE_SetInterruptNode(TLx493D_INTERRUPT_CCU4_SLICE, TLx493D_INTERRUPT_IRQ_EVENT_ID, TLx493D_INTERRUPT_IRQ_EVENT_ID_NUM);
    NVIC_SetPriority(TLx493D_INTERRUPT_CCU4_IRQ, 3);

    XMC_CCU4_SLICE_ConfigureEvent(TLx493D_INTERRUPT_CCU4_SLICE, TLx493D_INTERRUPT_SLICE_EVENT_ID, &event_config);

    NVIC_EnableIRQ(TLx493D_INTERRUPT_CCU4_IRQ);                                                    

    /* initialize sensor data structures */
    TLx493D_W2BW_init(&dut);

    // initialite communcation with default IIC address A0
    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);

     /** Here we're setting the thresholds for all three directions of the sensor.
     *  After this we enable the wake up mode as well as the interrupt mode.
     *  Both is necessary in order to enable the wake up feature of the sensor.
     * 
     *  The datasheet also recommends to disable the interrupt after it is triggered.
     *  The reason for that is that the sensor continues to trigger the interrupt
     *  as long as one of the thresholds is exceeded.
     */

    dut.functions->setDefaultConfig(&dut);
    dut.functions->setWakeUpThresholdsAsInteger(&dut, LOWER_THRESHOLD_X, UPPER_THRESHOLD_X, LOWER_THRESHOLD_Y, UPPER_THRESHOLD_Y, LOWER_THRESHOLD_Z, UPPER_THRESHOLD_Z );
    dut.functions->enableWakeUpMode(&dut);
    
    dut.functions->enableInterrupt(&dut);

    /** Here we are checking if the wake up is set correctly.
     *  This can be done due to an internal flag in the register, which is
     *  representing the status of the wake up.
     */
    print("isWakeUpEnabled : ");
    printf(dut.functions->isWakeUpEnabled(&dut) ? "enabled\n" : "not enabled\n");
    XMC_Delay(100);
    
    printf("setup done.\n");

    

    /** The code in the loop is constantly checking if the interrupt flag is set.
     *  If this condition is fulfilled, we're reading the temperature as well as the
     *  magnetic values in X, Y, Z-direction and printing them to the console.
     *  Afterwards we're reseting the flag and wait for two seconds.
     */
    while(1){
        if( intTriggered ) {
            double x = 0, y = 0, z = 0;
            dut.functions->getMagneticField(&dut, &x, &y, &z);
            printf("x = %f   y = %f   z = %f\n", x, y, z);

            double temperature = 0;
            dut.functions->getTemperature(&dut, &temperature);
            printf("temperature = %f\n", temperature);

            printRegisters(&dut, NULL);

            XMC_Delay(2000);

            intTriggered = false;
        }
    }
}


void CCU40_0_IRQHandler(void)
{
   intTriggered = true;
}

void CCU40_1_IRQHandler(void)
{
   intTriggered = true;
}