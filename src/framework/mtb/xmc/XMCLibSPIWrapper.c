// std includes
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

// MTB includes
#include "cybsp.h"
#include "cy_utils.h"

// XMC related includes
#include "xmc_gpio.h"
#include "xmc_spi.h"

// project c includes
// common to all sensors
#include "tlx493d_types.h"

// common to same generation of sensors

// sensor specific includes

// project c includes
#include "types.h"
#include "XMCLibSPIWrapper.h"

// static const uint8_t TLX493D_SPI_READ_BIT         = 0x80;
static const uint8_t TLX493D_XMC_SPI_READ_BIT_ON      = 0x80;
// static const uint8_t TLX493D_SPI_READ_BIT_OFF     = 0x00;

// static const uint8_t TLX493D_SPI_AUTO_INC_BIT     = 0x60;
// static const uint8_t TLX493D_SPI_AUTO_INC_BIT_ON  = 0x60;
// static const uint8_t TLX493D_SPI_AUTO_INC_BIT_OFF = 0x00;

// this constant always assumes the value = 2 for all devices (XMC4500, XMC1100, XMC47/4800) using a SPI interface located on Arduino header.
// Note: if some pins with special locations (on X1, X2 headers, for example) need to be used the corresponding INPUT SOURCE value needs to be
// passed to the XMC_SPI_CH_SetInputSource() function as an argument. 
static const uint8_t XMC_USICx_CHy_DX0CR_DSEL_VALUE = 2;

uint8_t spiReadAddress                              = 0x00;



bool tlx493d_xmc_initSPI(TLx493D_t *sensor){
    TLx493D_SPIObject_t *spi_obj = sensor->comInterface.comLibObj.spi_obj;
    XMC_USIC_CH_t *channel = spi_obj->channel;

    XMC_SPI_CH_Init(channel, (XMC_SPI_CH_CONFIG_t *)&TLx493D_SPI_config);
    XMC_SPI_CH_SetInputSource(channel, XMC_SPI_CH_INPUT_DIN0, XMC_USICx_CHy_DX0CR_DSEL_VALUE);

    XMC_SPI_CH_Start(channel);

    XMC_GPIO_Init(spi_obj->portSCK, spi_obj->pinSCK, &TLx493D_SPI_SCK_config);
    XMC_GPIO_Init(spi_obj->portMISO, spi_obj->pinMISO, &TLx493D_SPI_MISO_config);
    XMC_GPIO_Init(spi_obj->portMOSI, spi_obj->pinMOSI, &TLx493D_SPI_MOSI_config);

// for the XMC1100 Boot Kit, the only available USIC channel is shared by SPI and I2C interfaces
// by default, the channel is configured to be used as the I2C interface hence certain defines
// are hardcoded by MTB for I2C. As a a result, GPIO modes for SPI have to be additionally set for the XMC1100 Boot Kit.
#ifdef TARGET_APP_KIT_XMC11_BOOT_001    
    XMC_GPIO_SetMode(spi_obj->portSCK, spi_obj->pinSCK, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6);
    XMC_GPIO_SetMode(spi_obj->portMOSI, spi_obj->pinMOSI, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6);
#endif
    XMC_SPI_CH_ConfigureShiftClockOutput(channel, XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED, XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
    XMC_SPI_CH_SetBaudrate(channel, 2000000U);
    XMC_SPI_CH_SetBitOrderMsbFirst(channel);

    return true;
}

bool tlx493d_xmc_deinitSPI(TLx493D_t *sensor){
    TLx493D_SPIObject_t *spi_obj = sensor->comInterface.comLibObj.spi_obj;
    XMC_USIC_CH_t *channel = spi_obj->channel;

    XMC_SPI_CH_Stop(channel);

    return true;
}


bool tlx493d_xmc_transferSPI(TLx493D_t *sensor, uint8_t *txBuffer, uint8_t txLen, uint8_t *rxBuffer, uint8_t rxLen){
    if( sensor->boardSupportInterface.boardSupportObj.k2go_obj != NULL ) {
        bsc_controlSelect(sensor->boardSupportInterface.boardSupportObj.k2go_obj->k2go, true);
    }
    
    TLx493D_SPIObject_t *spi_obj = sensor->comInterface.comLibObj.spi_obj;
    XMC_USIC_CH_t *channel = spi_obj->channel;
    
    if((txLen > 0) && (txBuffer != NULL)){

        uint8_t bytesWritten = 0;

        for(; bytesWritten < txLen; ++bytesWritten){
            /* Clear RBUF0 and RBUF1 to receive into buffers while sending*/
            (void)XMC_SPI_CH_GetReceivedData(channel);
            (void)XMC_SPI_CH_GetReceivedData(channel);

            XMC_SPI_CH_Transmit(channel, txBuffer[bytesWritten], XMC_SPI_CH_MODE_STANDARD);

            while ((XMC_SPI_CH_GetStatusFlag(channel) & XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION) == 0U);
            XMC_SPI_CH_ClearStatusFlag(channel, XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION);
        }

        if( bytesWritten != txLen) {
            return false;
        }
    }

    if((rxLen > 0) && (rxBuffer != NULL)){
        
        uint16_t bytesRead = 0;

        XMC_SPI_CH_Transmit(channel, (TLX493D_XMC_SPI_READ_BIT_ON | spiReadAddress), XMC_SPI_CH_MODE_STANDARD);
        while ((XMC_SPI_CH_GetStatusFlag(channel) & XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION) == 0U);
        XMC_SPI_CH_ClearStatusFlag(channel, XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION);

        for(;bytesRead < rxLen; ++bytesRead){
            /* Clear RBUF0 and RBUF1 to receive into buffers while sending*/
            (void)XMC_SPI_CH_GetReceivedData(channel);
            (void)XMC_SPI_CH_GetReceivedData(channel);

            XMC_SPI_CH_Transmit(channel, (TLX493D_XMC_SPI_READ_BIT_ON | spiReadAddress), XMC_SPI_CH_MODE_STANDARD);

            while ((XMC_SPI_CH_GetStatusFlag(channel) & XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION) == 0U);
            XMC_SPI_CH_ClearStatusFlag(channel, XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION);

            while (XMC_USIC_CH_GetReceiveBufferStatus(channel) == 0U);

            rxBuffer[bytesRead] = XMC_SPI_CH_GetReceivedData(channel);

            XMC_SPI_CH_ClearStatusFlag(channel, ((uint32_t)XMC_SPI_CH_STATUS_FLAG_RECEIVE_INDICATION | (uint32_t)XMC_SPI_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION));
        }

        if( bytesRead != rxLen ) {
            return false;
        }
    }   

    if( sensor->boardSupportInterface.boardSupportObj.k2go_obj != NULL ) {
        bsc_controlSelect(sensor->boardSupportInterface.boardSupportObj.k2go_obj->k2go, false);
    }  

    return true;
}


void tlx493d_xmc_setReadAddressSPI(TLx493D_t *sensor, uint8_t address) {
    spiReadAddress = address;
}