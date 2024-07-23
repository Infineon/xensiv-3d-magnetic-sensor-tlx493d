// std includes
#include <malloc.h>
#include <stddef.h>

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"

// XMC related includes
#include "xmc_gpio.h"
#include "xmc_i2c.h"

// project c includes
// common to all sensors
#include "pal.h"
#include "types.h"
#include "XMCLibIICWrapper.h"
#include "IICUsingXMCLibIIC.h"


TLx493D_ComLibraryFunctions_t  comLibIF_i2c = {
                                            .init           = { .iic_init     = tlx493d_xmc_initIIC },
                                            .deinit         = { .iic_deinit   = tlx493d_xmc_deinitIIC },
                                            .transfer       = { .iic_transfer = tlx493d_xmc_transferIIC },
                                            .setReadAddress = { .iic_setReadAddress = tlx493d_xmc_setReadAddressIIC },
                                            };


bool tlx493d_initXMCIICCommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel,
                     const uint8_t sourceSDA, const uint8_t sourceSCL,
                     XMC_GPIO_PORT_t *const portSDA, const uint8_t pinSDA,
                     XMC_GPIO_PORT_t *const portSCL, const uint8_t pinSCL, TLx493D_IICAddressType_t iic_addr) {
    
    if( sensor->comIFType != TLx493D_I2C_e ) {
        return false;
    }

    // Need to dynamically allocate object, such that different sensor may use different channels and ports/pins
    sensor->comInterface.comLibObj.iic_obj                  = (TLx493D_I2CObject_t *) malloc(sizeof(TLx493D_I2CObject_t));
    sensor->comInterface.comLibObj.iic_obj->channel         = channel;
    sensor->comInterface.comLibObj.iic_obj->sourceSDA       = sourceSDA;
    sensor->comInterface.comLibObj.iic_obj->sourceSCL       = sourceSCL;
    sensor->comInterface.comLibObj.iic_obj->portSDA         = portSDA;
    sensor->comInterface.comLibObj.iic_obj->pinSDA          = pinSDA;
    sensor->comInterface.comLibObj.iic_obj->portSCL         = portSCL;
    sensor->comInterface.comLibObj.iic_obj->pinSCL          = pinSCL;

    sensor->comInterface.comLibFuncs                        = &comLibIF_i2c;

    sensor->comInterface.comLibParams.iic_params.address    = sensor->functions->selectIICAddress(sensor, iic_addr);

    sensor->comInterface.comLibFuncs->init.iic_init(sensor);

    return true;
}