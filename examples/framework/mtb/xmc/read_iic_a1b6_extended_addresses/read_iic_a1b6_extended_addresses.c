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

#include "read_iic_a1b6_extended_addresses.h"

// POWER PIN - arduino pin 8; ADDR PIN - arduino pin 7
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#define TLx493D_ADDR_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 9
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#define TLx493D_ADDR_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 4
#endif


void read_iic_a1b6_extended_addresses(){
    /*  board support object instantation and initilization of power pin and addr pin
        parameters. The addr pin is connected to the SDA pin of the sensor.
    */
    static Kit2GoBoardSupport k2go;
    bsc_initAttributes(&k2go);
    bsc_setPowerPin(&k2go, TLx493D_POWER_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 500000, 500000); //arduino pin 8
    bsc_setAddrPin(&k2go, TLx493D_ADDR_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
             XMC_GPIO_OUTPUT_LEVEL_LOW, XMC_GPIO_OUTPUT_LEVEL_HIGH, 1000,1000 );// arduino pin 7     
    
    // sensor object 
    TLx493D_t dut;
    
    /* Initialize the sensor data structures */
    TLx493D_A1B6_init(&dut);
    
    /* associate the board support object to the corresponding sensor object*/
    tlx493d_initBoardSupport(&dut, &k2go);

    /* power up the sensor and hold the ADDR/SDA line low 
       after 1 ms, the SDA/ADDR line is isolated by making it High-Z
    */    
    bsc_init(dut.boardSupportInterface.boardSupportObj.k2go_obj->k2go, true, false, true);

    // initialize communication - with IIC address A4 0x3E
    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A4_e);

    // Options to set the other remaining high/extended addresses
    // dut.functions->setIICAddress(&dut, TLx493D_IIC_ADDR_A5_e); // 0x36
    // dut.functions->setIICAddress(&dut, TLx493D_IIC_ADDR_A6_e); // 0x1E
    // dut.functions->setIICAddress(&dut, TLx493D_IIC_ADDR_A7_e); // 0x16

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