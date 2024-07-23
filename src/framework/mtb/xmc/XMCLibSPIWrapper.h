#ifndef TLX493D_SPI_USING_XMCLIB_H
#define TLX493D_SPI_USING_XMCLIB_H


// std includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// project c includes

// common to all sensors
#include "tlx493d_types.h"

bool tlx493d_xmc_initSPI(TLx493D_t *sensor);
bool tlx493d_xmc_deinitSPI(TLx493D_t *sensor);
bool tlx493d_xmc_transferSPI(TLx493D_t *sensor, uint8_t *txBuffer, uint8_t txLen, uint8_t *rxBuffer, uint8_t rxLen);
void tlx493d_xmc_setReadAddressSPI(TLx493D_t *sensor, uint8_t address); 
#endif // TLX493D_SPI_USING_XMCLIB_H
