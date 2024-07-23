#include "Test_includes.h"

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

//c includes
#include "IICUsingXMCLibIIC.h"
#include "SPIUsingXMCLibSPI.h"
#include "BoardSupportUsingKit2Go.h"


// project includes
#include "Test_TLx493D_A1B6_needsSensor.h"
#include "Test_tlx493d_commonFunctions_needsSensor.h"

// power pin define
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#define TLx493D_ADDR_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 9
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#define TLx493D_ADDR_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 4
#endif 

// powering up the board
static Kit2GoBoardSupport tlx493d_a1b6_bsc;

// Method invoked by Unity before a test suite is run 
void TLx493D_A1B6_needsSensor_suiteSetup() {
    // deinit in TEAR_DOWN will cut communication link, so if deinit is called communication must be reinitialized !
    print("\nbsc object used to power on board; check if POWER PIN connected!\n");
    // power up the board using bsc
    bsc_initAttributes(&tlx493d_a1b6_bsc);
    bsc_setPowerPin(&tlx493d_a1b6_bsc, TLx493D_POWER_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 50000, 50000); //arduino pin 8
    bsc_init(&tlx493d_a1b6_bsc, true, false, false);

    (void) TLx493D_A1B6_init(&dut);

    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    TLx493D_A1B6_setDefaultConfig(&dut);
}


// Method invoked by Unity after a test suite is run 
void TLx493D_A1B6_needsSensor_suiteTearDown() {
    // If deinitializing here make sure to reinit in 'TEST_SETUP' or communication will be lost !
    
    bsc_deinit(&tlx493d_a1b6_bsc);

    (void) TLx493D_A1B6_deinit(&dut);
}


// Method invoked by Unity before a test suite is run 
void TLx493D_A1B6_atReset_suiteSetup() {
    // deinit in TEAR_DOWN will cut communication link, so if deinit is called communication must be reinitialized !
    
    // power up the board 
    bsc_init(&tlx493d_a1b6_bsc, true, false, false);

    (void) TLx493D_A1B6_init(&dut);

    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);

    tlx493d_readRegisters(&dut);
}


// Method invoked by Unity after a test suite is run 
void TLx493D_A1B6_atReset_suiteTearDown() {

    // power down the board 
    bsc_deinit(&tlx493d_a1b6_bsc);

    (void) TLx493D_A1B6_deinit(&dut);
}

// Method invoked by Unity before a test suite is run 
void TLx493D_A1B6_extendedAddresses_suiteSetup() {
    // deinit in TEAR_DOWN will cut communication link, so if deinit is called communication must be reinitialized !
    print("\nbsc object used to power on board; check if POWER PIN connected!\ncheck if ADDR pin is connected!\n");
    // power up the board using bsc
    bsc_setAddrPin(&tlx493d_a1b6_bsc, TLx493D_ADDR_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
             XMC_GPIO_OUTPUT_LEVEL_LOW, XMC_GPIO_OUTPUT_LEVEL_HIGH, 1000,1000 );// arduino pin 7    
    
    bsc_init(&tlx493d_a1b6_bsc, true, false, true);

    (void) TLx493D_A1B6_init(&dut);

    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A4_e); //address A4
    TLx493D_A1B6_setDefaultConfig(&dut);
}


// Method invoked by Unity after a test suite is run 
void TLx493D_A1B6_extendedAddresses_suiteTearDown() {
    // If deinitializing here make sure to reinit in 'TEST_SETUP' or communication will be lost !
    
    bsc_deinit(&tlx493d_a1b6_bsc);

    (void) TLx493D_A1B6_deinit(&dut);
}