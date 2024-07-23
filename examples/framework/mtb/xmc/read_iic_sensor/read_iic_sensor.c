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

#include "TLx493D_A1B6.h"
#include "TLx493D_A2B6.h"
#include "TLx493D_P2B6.h"
#include "TLx493D_W2B6.h"
#include "TLx493D_W2BW.h"
#include "TLx493D_P3B6.h"


#include "read_iic_sensor.h"

// POWER PIN - arduino pin 8
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#endif


void read_iic_sensor(){
    // powering up the board
    static Kit2GoBoardSupport k2go;
    bsc_initAttributes(&k2go);
    bsc_setPowerPin(&k2go, TLx493D_POWER_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000); //arduino pin 8
    
    bsc_init(&k2go, true, false, false);

    printf("Power-up done\n");

    // sensor object
    TLx493D_t dut;

    /* Uncomment below the sensor-type to be used */

    // TLx493D_A1B6_init(&dut);
    TLx493D_A2B6_init(&dut);
    // TLx493D_P2B6_init(&dut);
    // TLx493D_W2B6_init(&dut);
    // TLx493D_W2BW_init(&dut);
    // TLx493D_P3B6_init(&dut);

    // initialite communcation with default IIC address A0
    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);

    // set the default values
    dut.functions->setDefaultConfig(&dut);

    printf("setup done\n");

    // query values from the sensor in a loop
    while(1){
        double x = 0, y = 0, z = 0;
        dut.functions->getMagneticField(&dut, &x, &y, &z);
        printf("x = %f   y = %f   z = %f\n", x, y, z);

        double temperature = 0;
        dut.functions->getTemperature(&dut, &temperature);
        printf("temperature = %f\n", temperature);

        printRegisters(&dut, NULL);

        XMC_Delay(1000);
    }
}