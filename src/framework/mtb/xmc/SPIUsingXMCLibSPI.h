#ifndef TLX493D_SPI_USING_XMCLIB_SPI_H
#define TLX493D_SPI_USING_XMCLIB_SPI_H

// std includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// project c includes
// common to all sensors
#include "tlx493d_types.h"

bool tlx493d_initXMCSPICommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel, 
                     XMC_GPIO_PORT_t *const portMOSI, const uint8_t pinMOSI,
                     XMC_GPIO_PORT_t *const portMISO, const uint8_t pinMISO,
                     XMC_GPIO_PORT_t *const portSCK, const uint8_t pinSCK);                     

#endif // TLX493D_SPI_USING_XMCLIB_SPI_H
