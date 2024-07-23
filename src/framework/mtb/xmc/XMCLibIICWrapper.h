#ifndef TLX493D_IIC_USING_TWOWIRE_H
#define TLX493D_IIC_USING_TWOWIRE_H

// std includes
#include <stdint.h>

// project c includes
// common to all sensors
#include "tlx493d_types.h"

bool tlx493d_xmc_initIIC(TLx493D_t *sensor);

bool tlx493d_xmc_deinitIIC(TLx493D_t *sensor);

bool tlx493d_xmc_transferIIC(TLx493D_t *sensor, uint8_t *txBuffer, uint8_t txLen, uint8_t *rxBuffer, uint8_t rxLen);

void tlx493d_xmc_setReadAddressIIC(TLx493D_t *sensor, uint8_t address) ;
#endif // TLX493D_IIC_USING_TWOWIRE_H
