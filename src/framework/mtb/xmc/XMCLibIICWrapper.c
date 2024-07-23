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
#include "Logger.h"



bool tlx493d_xmc_initIIC(TLx493D_t *sensor) {
    TLx493D_I2CObject_t  *iic_obj = sensor->comInterface.comLibObj.iic_obj;
    XMC_USIC_CH_t *channel = iic_obj->channel;

    XMC_I2C_CH_Init(channel, (XMC_I2C_CH_CONFIG_t *)&TLx493D_IIC_config);

    XMC_I2C_CH_SetInputSource(channel, XMC_I2C_CH_INPUT_SCL, iic_obj->sourceSCL);
    XMC_I2C_CH_SetInputSource(channel, XMC_I2C_CH_INPUT_SDA, iic_obj->sourceSDA);

	XMC_I2C_CH_ClearStatusFlag(channel, 0xFFFFFFFF);
    XMC_I2C_CH_Start(channel);

    XMC_GPIO_Init(iic_obj->portSCL, iic_obj->pinSCL, &TLx493D_IIC_SCL_config);
    XMC_GPIO_Init(iic_obj->portSDA, iic_obj->pinSDA, &TLx493D_IIC_SDA_config);

    return true;
}


bool tlx493d_xmc_deinitIIC(TLx493D_t *sensor) {
    TLx493D_I2CObject_t  *iic_obj = sensor->comInterface.comLibObj.iic_obj;
    XMC_USIC_CH_t *channel = iic_obj->channel;

    XMC_I2C_CH_Stop(channel);
    XMC_I2C_CH_ClearStatusFlag(channel, 0xFFFFFFFF);

    return true;
}


bool tlx493d_xmc_transferIIC(TLx493D_t *sensor, uint8_t *tx_buffer, uint8_t tx_len, uint8_t *rx_buffer, uint8_t rx_len) {
    XMC_USIC_CH_t *channel = sensor->comInterface.comLibObj.iic_obj->channel;

    if( tx_buffer != NULL ) {
    	XMC_I2C_CH_ClearStatusFlag(channel, 0xFFFFFFFF);
        XMC_I2C_CH_MasterStart(channel, sensor->comInterface.comLibParams.iic_params.address, XMC_I2C_CH_CMD_WRITE);

        while((XMC_I2C_CH_GetStatusFlag(channel) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
        {
        }

        XMC_I2C_CH_ClearStatusFlag(channel, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

        for(uint8_t n = 0; n < tx_len; ++n) {
            XMC_I2C_CH_MasterTransmit(channel, tx_buffer[n]);

            while((XMC_I2C_CH_GetStatusFlag(channel) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
            {
            }

            XMC_I2C_CH_ClearStatusFlag(channel, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
        }

        while (!XMC_USIC_CH_TXFIFO_IsEmpty(channel))
        {
        }

        XMC_I2C_CH_MasterStop(channel);

        while ( XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY )
		{
		}
    }

    if( rx_buffer != NULL ) {
      	XMC_I2C_CH_ClearStatusFlag(channel, 0xFFFFFFFF);
        XMC_I2C_CH_MasterStart(channel, sensor->comInterface.comLibParams.iic_params.address, XMC_I2C_CH_CMD_READ);

        // this loop solves the incorrect reads present in Arduino
        while((XMC_I2C_CH_GetStatusFlag(channel) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
        {
        }

        XMC_I2C_CH_ClearStatusFlag(channel, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

        for(uint8_t n = 0; n < (rx_len - 1); ++n) {
            XMC_I2C_CH_MasterReceiveAck(channel);

    		while((XMC_I2C_CH_GetStatusFlag(channel) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U)
    		{
    		}

    		XMC_I2C_CH_ClearStatusFlag(channel, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);
            rx_buffer[n] = XMC_I2C_CH_GetReceivedData(channel);
        }

		XMC_I2C_CH_MasterReceiveNack(channel);

        // this loop solves the incorrect reads present in Arduino
		while((XMC_I2C_CH_GetStatusFlag(channel) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U)
		{
		}

		XMC_I2C_CH_ClearStatusFlag(channel, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

        if( rx_len > 0 ) {
		    rx_buffer[rx_len - 1] = XMC_I2C_CH_GetReceivedData(channel);
        }

        XMC_I2C_CH_MasterStop(channel);

        while ( XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY )
		{
		}

    }

    return true;
}

void tlx493d_xmc_setReadAddressIIC(TLx493D_t *sensor, uint8_t address) {
    warn("Function 'tlx493d_setReadAddressIIC' not supported !");
}
