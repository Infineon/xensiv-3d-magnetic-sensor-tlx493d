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
#include "Test_TLx493D_A1B6.h"

// power pin define
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#endif

// powering up the board
static Kit2GoBoardSupport tlx493d_a1b6_bsc;

// Method invoked by Unity before a test suite is run
void TLx493D_A1B6_suiteSetUp() {
    print("bsc object used to power on board\n");
     // power up the board using bsc
    bsc_initAttributes(&tlx493d_a1b6_bsc);
    bsc_setPowerPin(&tlx493d_a1b6_bsc, TLx493D_POWER_PIN_PORT_AND_NUM, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, 
        XMC_GPIO_OUTPUT_LEVEL_HIGH, XMC_GPIO_OUTPUT_LEVEL_LOW, 50000, 50000); //arduino pin 8
    bsc_init(&tlx493d_a1b6_bsc, true, false, false);

}


// Method invoked by Unity after a test suite is run 
void TLx493D_A1B6_suiteTearDown() {
    // power down the board
    bsc_deinit(&tlx493d_a1b6_bsc);
}
