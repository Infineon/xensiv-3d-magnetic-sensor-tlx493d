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
#include "xmc_gpio.h"
#include "xmc_spi.h"

// project c includes
#include "types.h"
#include "tlx493d_types.h"
#include "BoardSupportUsingKit2Go.h"
#include "Logger.h"

//#include "tlx493d_common.h"
#include "TLx493D_P3I8.h"

#include "read_spi_sensor.h"

// POWER PIN - arduino pin 8
#if defined (TARGET_APP_KIT_XMC47_RELAX_V1) || defined (TARGET_APP_KIT_XMC48_RELAX_ECAT_V1)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT1, 10
#elif defined(TARGET_APP_KIT_XMC11_BOOT_001)
#define TLx493D_POWER_PIN_PORT_AND_NUM XMC_GPIO_PORT0, 12
#endif

void read_spi_sensor() {
    
    TLx493D_t p3i8;

    TLx493D_P3I8_init(&p3i8);

    static Kit2GoBoardSupport bsc;
    bsc_initAttributes(&bsc);
    bsc_setSelectPin(&bsc, TLx493D_SPI_SS_PORT, TLx493D_SPI_SS_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL, XMC_GPIO_OUTPUT_LEVEL_LOW, XMC_GPIO_OUTPUT_LEVEL_HIGH, 50, 50);
    bsc_begin(&bsc, false, false, false);
    tlx493d_initBoardSupport(&p3i8, &bsc);

    tlx493d_initXMCSPICommunication(&p3i8, TLx493D_SPI_HW, TLx493D_SPI_MOSI_PORT, TLx493D_SPI_MOSI_PIN, 
            TLx493D_SPI_MISO_PORT, TLx493D_SPI_MISO_PIN, TLx493D_SPI_SCK_PORT, TLx493D_SPI_SCK_PIN);

    TLx493D_P3I8_setDefaultConfig(&p3i8);

    while( true ) {
        double x = 0, y = 0, z = 0;
        TLx493D_P3I8_getMagneticField(&p3i8, &x, &y, &z);
        printf("x = %f   y = %f   z = %f\n", x, y, z);

        double temperature = 0;
        TLx493D_P3I8_getTemperature(&p3i8, &temperature);
        printf("temperature = %f °C\n", temperature);

        int16_t raw_temperature;
        TLx493D_P3I8_getRawTemperature(&p3i8, &raw_temperature);
        printf("raw temperature = %d LSB\n", raw_temperature);

        // printRegisters(&p3i8);

        XMC_Delay(1000);
   }
}
