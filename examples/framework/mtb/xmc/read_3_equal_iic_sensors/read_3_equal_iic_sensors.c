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

#include "read_3_equal_iic_sensors.h"

// POWER PIN - arduino pin 8,9,10
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM_1 XMC_GPIO_PORT1, 10
#define TLx493D_POWER_PIN_PORT_AND_NUM_2 XMC_GPIO_PORT1, 11
#define TLx493D_POWER_PIN_PORT_AND_NUM_3 XMC_GPIO_PORT3, 10
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM_1 XMC_GPIO_PORT0, 12
#define TLx493D_POWER_PIN_PORT_AND_NUM_2 XMC_GPIO_PORT0, 8
#define TLx493D_POWER_PIN_PORT_AND_NUM_3 XMC_GPIO_PORT0, 9
#endif


void read_3_equal_iic_sensors(){
    // instantiate board support objects and initialize them
    static Kit2GoBoardSupport bsc1;
    static Kit2GoBoardSupport bsc2;
    static Kit2GoBoardSupport bsc3;
    bsc_initAttributes(&bsc1);
    bsc_initAttributes(&bsc2);
    bsc_initAttributes(&bsc3);

    /** Declaration of three sensor objects  */
    TLx493D_t dut1;
    TLx493D_t dut2;
    TLx493D_t dut3;

    /** In this example we're using the XMC4700-Relax-Kit. 
     *  Here we're using three GPIOs to power up the sensors, one
     *  after the other. This is done by defining the pins with the
     *  Board Support Structure and its related functions. The power up pins are
     *  enabled during the SensorClassObject.init() call. See below. 
    */
    bsc_setPowerPin(&bsc1, TLx493D_POWER_PIN_PORT_AND_NUM_1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000); 
    bsc_setPowerPin(&bsc2, TLx493D_POWER_PIN_PORT_AND_NUM_2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000);
    bsc_setPowerPin(&bsc3, TLx493D_POWER_PIN_PORT_AND_NUM_3, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000);            

    /* initalize the sensors using the sensor objects declared above*/
    TLx493D_W2BW_init(&dut1);
    TLx493D_W2BW_init(&dut2);
    TLx493D_W2BW_init(&dut3);

    /* associating board support objs to their corresponding sensors*/
    tlx493d_initBoardSupport(&dut1, &bsc1);
    tlx493d_initBoardSupport(&dut2, &bsc2);
    tlx493d_initBoardSupport(&dut3, &bsc3);
    
    // initialite communcation with default IIC address A0 for all 3 sensors
    tlx493d_initXMCIICCommunication(&dut1, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    tlx493d_initXMCIICCommunication(&dut2, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    tlx493d_initXMCIICCommunication(&dut3, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);                

    /** Here we're enabling one sensor after the other. This procedure 
     *  is necessary, otherwise it can happen that the initialization of the
     *  sensors fails. In between we change the I2C-Address of the sensor
     *  that was just enabled. This has to be done, because the default 
     *  addresses of the senors are identical. For the last sensor this is not 
     *  required anymore, since the other two have a new address.
    */
    bsc_init(dut1.boardSupportInterface.boardSupportObj.k2go_obj->k2go, true, false, false);
    dut1.functions->setDefaultConfig(&dut1);  
    dut1.functions->setIICAddress(&dut1, TLx493D_IIC_ADDR_A2_e);
    XMC_Delay(500);
    bsc_init(dut2.boardSupportInterface.boardSupportObj.k2go_obj->k2go, true, false, false);
    dut2.functions->setDefaultConfig(&dut2);
    dut2.functions->setIICAddress(&dut2, TLx493D_IIC_ADDR_A1_e);
    XMC_Delay(500);
    bsc_init(dut3.boardSupportInterface.boardSupportObj.k2go_obj->k2go, true, false, false);
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