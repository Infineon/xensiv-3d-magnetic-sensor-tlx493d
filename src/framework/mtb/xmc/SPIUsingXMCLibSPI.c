// std includes
#include <malloc.h>
#include <stddef.h>

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"

// XMC includes
#include "xmc_gpio.h"
#include "xmc_spi.h"

// project c includes
// common to all sensors
#include "tlx493d_types.h"

// common to same generation of sensors

// sensor specific includes

// project c includes
#include "types.h"
#include "Logger.h"
#include "XMCLibSPIWrapper.h"
#include "Kit2GoBoardSupport.h"
#include "XMCLibSPIWrapper.h"

#include "SPIUsingXMCLibSPI.h"


TLx493D_ComLibraryFunctions_t  comLibFuncs_spi = {
                                            .init           = { .spi_init           = tlx493d_xmc_initSPI },
                                            .deinit         = { .spi_deinit         = tlx493d_xmc_deinitSPI },
                                            .transfer       = { .spi_transfer       = tlx493d_xmc_transferSPI },
                                            .setReadAddress = { .spi_setReadAddress = tlx493d_xmc_setReadAddressSPI },
                                       };

bool tlx493d_initXMCSPICommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel, 
                     XMC_GPIO_PORT_t *const portMOSI, const uint8_t pinMOSI,
                     XMC_GPIO_PORT_t *const portMISO, const uint8_t pinMISO,
                     XMC_GPIO_PORT_t *const portSCK, const uint8_t pinSCK) {
    if (sensor->comIFType != TLx493D_SPI_e){
        return false;
    }
    sensor->comInterface.comLibObj.spi_obj = (TLx493D_SPIObject_t *) malloc(sizeof(TLx493D_SPIObject_t));
    sensor->comInterface.comLibObj.spi_obj->channel = channel;
    sensor->comInterface.comLibObj.spi_obj->portMOSI = portMOSI;
    sensor->comInterface.comLibObj.spi_obj->pinMOSI = pinMOSI;
    sensor->comInterface.comLibObj.spi_obj->portMISO = portMISO;
    sensor->comInterface.comLibObj.spi_obj->pinMISO = pinMISO;
    sensor->comInterface.comLibObj.spi_obj->portSCK = portSCK;
    sensor->comInterface.comLibObj.spi_obj->pinSCK = pinSCK;

    sensor->comInterface.comLibFuncs = &comLibFuncs_spi;

    sensor->comInterface.comLibFuncs->init.spi_init(sensor);

    return true;
}


