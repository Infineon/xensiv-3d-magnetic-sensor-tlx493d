#ifndef TYPES_H
#define TYPES_H


// std includes
#include <stdint.h>

// XMC includes
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_spi.h"

// project cpp includes
#include "Kit2GoBoardSupport.h"
//#include "TLx493D.hpp"

// project c includes
// common to all sensors
#include "tlx493d_types.h"

typedef struct TLx493D_I2CObject_t {
    XMC_USIC_CH_t    *channel;
    uint8_t           sourceSDA;
    uint8_t           sourceSCL;
    XMC_GPIO_PORT_t  *portSDA;
    uint8_t           pinSDA;
    XMC_GPIO_PORT_t  *portSCL;
    uint8_t           pinSCL;
    bool              isToBeDeleted;
} TLx493D_I2CObject_t;

typedef struct TLx493D_SPIObject_t {
    XMC_USIC_CH_t    *channel;
    XMC_GPIO_PORT_t  *portMOSI;
    uint8_t           pinMOSI;
    XMC_GPIO_PORT_t  *portMISO;
    uint8_t           pinMISO;
    XMC_GPIO_PORT_t  *portSCK;
    uint8_t           pinSCK;
    bool              isToBeDeleted;
} TLx493D_SPIObject_t;


typedef struct TLx493D_Kit2GoBoardSupportObject_t {
    Kit2GoBoardSupport  *k2go;
    bool                 isToBeDeleted;
} TLx493D_Kit2GoBoardSupportObject_t;


extern bool tlx493d_initXMCIICCommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel,
                            const uint8_t sourceSDA, const uint8_t sourceSCL,
                            XMC_GPIO_PORT_t *const portSDA, const uint8_t pinSDA,
                            XMC_GPIO_PORT_t *const portSCL, const uint8_t pinSCL, TLx493D_IICAddressType_t iic_addr);

extern bool tlx493d_initXMCSPICommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel, 
                     XMC_GPIO_PORT_t *const portMOSI, const uint8_t pinMOSI,
                     XMC_GPIO_PORT_t *const portMISO, const uint8_t pinMISO,
                     XMC_GPIO_PORT_t *const portSCK, const uint8_t pinSCK);


#endif // TYPES_H
