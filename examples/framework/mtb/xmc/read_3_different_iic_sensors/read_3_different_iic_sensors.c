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

// project c includes
#include "types.h"
#include "tlx493d_types.h"
#include "BoardSupportUsingKit2Go.h"
#include "Logger.h"

#include "TLx493D_W2BW.h"
#include "TLx493D_A1B6.h"
#include "TLx493D_A2B6.h"

#include "read_3_different_iic_sensors.h"

// POWER PIN - arduino pin 8,9,10 -- todo: maybe needed for power up?
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM_1 XMC_GPIO_PORT1, 10
#define TLx493D_POWER_PIN_PORT_AND_NUM_2 XMC_GPIO_PORT1, 11
#define TLx493D_POWER_PIN_PORT_AND_NUM_3 XMC_GPIO_PORT3, 10
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM_1 XMC_GPIO_PORT0, 12
#define TLx493D_POWER_PIN_PORT_AND_NUM_2 XMC_GPIO_PORT0, 8
#define TLx493D_POWER_PIN_PORT_AND_NUM_3 XMC_GPIO_PORT0, 9
#endif


void read_3_different_iic_sensors(){

    /* Declaration of three sensor objects  */
    TLx493D_t dut1;
    TLx493D_t dut2;
    TLx493D_t dut3;

    /*  initalize the sensors using the sensor objects declared above. 
        One of the second generation and two of the third generation. 
    */
    TLx493D_W2BW_init(&dut1);
    TLx493D_A1B6_init(&dut2);
    TLx493D_A2B6_init(&dut3);
    
    /*  initialite communcation with IIC addresses
        The two sensors of the third generation have different default addresses.
        
     * Here we don't need a special routine for the initialization of the sensors,
     *  because only the second generation is critical in this case. So you have
     *  to make sure to initialize this sensor first. Otherwise it will trigger an
     *  interrupt on the interrupt line (default config of the sensor) which will
     *  mess up the sensor's communication interface. 
     */
    // original example with P2B6 A0 and A1 so both have different addresses + W2BW (different address)
    // currently tested with W2BW, A1B6 and A2B6 
    // todo: test with the other sensors as in the original example
    tlx493d_initXMCIICCommunication(&dut1, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    tlx493d_initXMCIICCommunication(&dut2, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    tlx493d_initXMCIICCommunication(&dut3, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);       

    dut1.functions->setDefaultConfig(&dut1);
    dut2.functions->setDefaultConfig(&dut2);
    dut3.functions->setDefaultConfig(&dut3);             

    printf("Setup done\n");

    /** In the loop we're reading out the temperature values as well as the magnetic values in X, Y, Z-direction 
     *  of all three sensors. After that they're all printed to the serial output.
     */
    while(1){
        double temp1 = 0.0, temp2 = 0.0, temp3 = 0.0;
        double valX1 = 0, valY1 = 0, valZ1 = 0, valX2 = 0, valY2 = 0, valZ2 = 0, valX3 = 0, valY3 = 0, valZ3 = 0;

        dut1.functions->getTemperature(&dut1, &temp1); 
        dut2.functions->getTemperature(&dut2, &temp2); 
        dut3.functions->getTemperature(&dut3, &temp3);

        dut1.functions->getMagneticField(&dut1, &valX1, &valY1, &valZ1);
        dut2.functions->getMagneticField(&dut2, &valX2, &valY2, &valZ2);
        dut3.functions->getMagneticField(&dut3, &valX3, &valY3, &valZ3);

        printf("========================================\n");
        printf("Temperature of Sensor 1:\t");printf("%f", temp1);printf(" °C\n");
        printf("Magnetic X-Value of Sensor 1:\t");printf("%f", valX1);printf(" mT\n");
        printf("Magnetic Y-Value of Sensor 1:\t");printf("%f", valY1);printf(" mT\n");
        printf("Magnetic Z-Value of Sensor 1:\t");printf("%f", valZ1);printf(" mT\n");
        printf("----------------------------------------\n");
        printf("Temperature of Sensor 2:\t");printf("%f", temp2);printf(" °C\n");
        printf("Magnetic X-Value of Sensor 2:\t");printf("%f", valX2);printf(" mT\n");
        printf("Magnetic Y-Value of Sensor 2:\t");printf("%f", valY2);printf(" mT\n");
        printf("Magnetic Z-Value of Sensor 2:\t");printf("%f", valZ2);printf(" mT\n");
        printf("----------------------------------------\n");
        printf("Temperature of Sensor 3:\t");printf("%f",temp3);printf(" °C\n");
        printf("Magnetic X-Value of Sensor 3:\t");printf("%f", valX3);printf(" mT\n");
        printf("Magnetic Y-Value of Sensor 3:\t");printf("%f", valY3);printf(" mT\n");
        printf("Magnetic Z-Value of Sensor 3:\t");printf("%f", valZ3);printf(" mT\n");
        printf("========================================\n\n");

        XMC_Delay(2000);
    }
}