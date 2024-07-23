// test includes
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
#include "Test_TLx493D_P3I8_needsSensor.h"
#include "Test_tlx493d_commonFunctions_needsSensor.h"

// Method invoked by Unity before a test suite is run 
void TLx493D_P3I8_needsSensor_suiteSetup() {
    // deinit in TEAR_DOWN will cut communication link, so if deinit is called communication must be reinitialized !
    (void) TLx493D_P3I8_init(&dut);

    static Kit2GoBoardSupport bsc;
    bsc_initAttributes(&bsc);
    bsc_setSelectPin(&bsc, TLx493D_SPI_SS_PORT, TLx493D_SPI_SS_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
                XMC_GPIO_OUTPUT_LEVEL_LOW, XMC_GPIO_OUTPUT_LEVEL_HIGH, 50, 50);
    bsc_begin(&bsc, false, false, false);
    tlx493d_initBoardSupport(&dut, &bsc);

    tlx493d_initXMCSPICommunication(&dut, TLx493D_SPI_HW, TLx493D_SPI_MOSI_PORT, TLx493D_SPI_MOSI_PIN, 
            TLx493D_SPI_MISO_PORT, TLx493D_SPI_MISO_PIN, TLx493D_SPI_SCK_PORT, TLx493D_SPI_SCK_PIN);
    
    TLx493D_P3I8_setDefaultConfig(&dut);
}


// Method invoked by Unity after a test suite is run 
void TLx493D_P3I8_needsSensor_suiteTearDown() {
    // If deinitializing here make sure to reinit in 'TEST_SETUP' or communication will be lost !
    dut.functions->deinit(&dut);
}

