// MTB includes
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

// XMC includes
#include "xmc_gpio.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"

// project c includes
#include "types.h"
#include "tlx493d_types.h"
#include "BoardSupportUsingKit2Go.h"

// test includes
#include "Test_includes.h"

void RunAllTests(void){

// TLx493D_A1B6
#ifdef TEST_TLx493D_A1B6

    RUN_TEST_GROUP(TLx493D_A1B6);

#endif

#ifdef TEST_TLx493D_A1B6_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_A1B6_needsSensor);

#endif


// TLx493D_A2B6
#ifdef TEST_TLx493D_A2B6

    RUN_TEST_GROUP(TLx493D_A2B6);

#endif

#ifdef TEST_TLx493D_A2B6_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_A2B6_needsSensor);

#endif


// TLx493D_P2B6
#ifdef TEST_TLx493D_P2B6

    RUN_TEST_GROUP(TLx493D_P2B6);

#endif

#ifdef TEST_TLx493D_P2B6_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_P2B6_needsSensor);

#endif


// TLx493D_W2B6
#ifdef TEST_TLx493D_W2B6

    RUN_TEST_GROUP(TLx493D_W2B6);

#endif

#ifdef TEST_TLx493D_W2B6_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_W2B6_needsSensor);

#endif


// TLx493D_W2BW
#ifdef TEST_TLx493D_W2BW

    RUN_TEST_GROUP(TLx493D_W2BW);

#endif

#ifdef TEST_TLx493D_W2BW_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_W2BW_needsSensor);

#endif


// TLx493D_P3B6
#ifdef TEST_TLx493D_P3B6

    RUN_TEST_GROUP(TLx493D_P3B6);

#endif

#ifdef TEST_TLx493D_P3B6_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_P3B6_needsSensor);

#endif


// TLx493D_P3B6
#ifdef TEST_TLx493D_P3I8

    RUN_TEST_GROUP(TLx493D_P3I8);

#endif

#ifdef TEST_TLx493D_P3I8_NEEDS_SENSOR

    RUN_TEST_GROUP(TLx493D_P3I8_needsSensor);

#endif
}


int main(int argc, char *argv[]) {

    /* Initialize the device and board peripherals */
    cy_rslt_t result = cybsp_init();

    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }

    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    printf("init begin\n");


    while(1){
        printf("\n");

        const int argc = 2;

        const char *argv[2] = { "", "-v" };

        (void) UnityMain(argc, argv, RunAllTests);

        XMC_Delay(3000);
    }
    
}







