
// std includes
#include <stdint.h>

// XMC includes
#include "xmc_gpio.h"
#include "xmc_i2c.h"

// project c includes
// common to all sensors
#include "tlx493d_types.h"


bool tlx493d_initXMCIICCommunication(TLx493D_t *sensor, XMC_USIC_CH_t *const channel,
                     const uint8_t sourceSDA, const uint8_t sourceSCL,
                     XMC_GPIO_PORT_t *const portSDA, const uint8_t pinSDA,
                     XMC_GPIO_PORT_t *const portSCL, const uint8_t pinSCL, TLx493D_IICAddressType_t iic_addr);

