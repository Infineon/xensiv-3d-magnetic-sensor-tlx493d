// test includes
#include "Test_includes.h"

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

//c includes
#include "IICUsingXMCLibIIC.h"
#include "SPIUsingXMCLibSPI.h"

// project includes
#include "Test_TLx493D_W2BW_needsSensor.h"
#include "Test_tlx493d_commonFunctions_needsSensor.h"


// Method invoked by Unity before a test suite is run 
void TLx493D_W2BW_needsSensor_suiteSetup() {
    // deinit in TEAR_DOWN will cut communication link, so if deinit is called communication must be reinitialized !
    (void) TLx493D_W2BW_init(&dut);
    tlx493d_initXMCIICCommunication(&dut, TLx493D_IIC_HW, TLx493D_IIC_DX0_INPUT, TLx493D_IIC_DX1_INPUT, TLx493D_IIC_SDA_PORT, TLx493D_IIC_SDA_PORT_NUM,  
        TLx493D_IIC_SCL_PORT, TLx493D_IIC_SCL_PORT_NUM, TLx493D_IIC_ADDR_A0_e);
    dut.functions->setDefaultConfig(&dut);
}


// Method invoked by Unity after a test suite is run 
void TLx493D_W2BW_needsSensor_suiteTearDown() {
    // If deinitializing here make sure to reinit in 'TEST_SETUP' or communication will be lost !
    dut.functions->deinit(&dut);
}

