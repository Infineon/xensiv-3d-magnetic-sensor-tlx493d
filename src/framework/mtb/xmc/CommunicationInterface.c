// std includes
#include <stdbool.h>
#include <stdlib.h>

// Arduino includes
#include "types.h"

// project c includes
// common to all sensors
#include "tlx493d_types.h"

// common to same generation of sensors

// sensor specific includes

// project cpp includes
#include "XMCLibIICWrapper.h"
#include "XMCLibSPIWrapper.h"


bool tlx493d_transfer(TLx493D_t *sensor, uint8_t *txBuffer, uint8_t txLen, uint8_t *rxBuffer, uint8_t rxLen) {
    return sensor->comIFType == TLx493D_I2C_e ? sensor->comInterface.comLibFuncs->transfer.iic_transfer(sensor, txBuffer, txLen, rxBuffer, rxLen)
                                              : (sensor->comIFType == TLx493D_SPI_e ? sensor->comInterface.comLibFuncs->transfer.spi_transfer(sensor, txBuffer, txLen, rxBuffer, rxLen)
                                                                                    : false);
}

void tlx493d_setReadAddress(TLx493D_t *sensor, uint8_t address) {
    if( sensor->comIFType == TLx493D_I2C_e ) {
        sensor->comInterface.comLibFuncs->setReadAddress.iic_setReadAddress(sensor, address);
    }
    else if( sensor->comIFType == TLx493D_SPI_e ) {
        sensor->comInterface.comLibFuncs->setReadAddress.spi_setReadAddress(sensor, address);
    }
}

void tlx493d_deinitCommunication(TLx493D_t *sensor) {
    if( sensor->comIFType == TLx493D_I2C_e ) {
        sensor->comInterface.comLibFuncs->deinit.iic_deinit(sensor);

        free(sensor->comInterface.comLibObj.iic_obj);
        sensor->comInterface.comLibObj.iic_obj = NULL;
    }
    else if( sensor->comIFType == TLx493D_SPI_e ) {
        sensor->comInterface.comLibFuncs->deinit.spi_deinit(sensor);
        
        free(sensor->comInterface.comLibObj.spi_obj);
        sensor->comInterface.comLibObj.spi_obj = NULL;
    }

    sensor->comInterface.comLibFuncs = NULL;
}

//todo: add deinit board support
