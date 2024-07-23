/*!
 * \name        main.c
 * \author      Infineon Technologies AG
 * \copyright   2024 Infineon Technologies AG
 * \brief       his is the source code for TLx493D Application
 *              for ModusToolbox.
 * \details		See README.md
 */

#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"


#include "examples/read_spi_sensor/read_spi_sensor.h"
#include "examples/read_iic_sensor/read_iic_sensor.h"
#include "examples/read_iic_sensor_plain_c/read_iic_sensor_plain_c.h"
#include "examples/read_iic_sensor_with_wakeup/read_iic_sensor_with_wakeup.h"
#include "examples/read_3_equal_iic_sensors/read_3_equal_iic_sensors.h"
#include "examples/read_3_different_iic_sensors/read_3_different_iic_sensors.h"
#include "examples/read_iic_a1b6_extended_addresses/read_iic_a1b6_extended_addresses.h"

//!< This is a list of all examples which are covered with here.
enum examples_t
{
	READ_SPI_SENSOR            = 1,      
	READ_IIC_SENSOR,                  
	READ_IIC_SENSOR_PLAIN_C,                
	READ_IIC_SENSOR_WITH_WAKEUP,
	READ_3_EQUAL_IIC_SENSORS,
	READ_3_DIFFERENT_IIC_SENSORS,
	READ_IIC_A1B6_EXTENDED_ADDRESSES
};

//!> Select the example which is compiled. There are a number of different examples defined which can be switched here.
#ifndef EXAMPLE
#define EXAMPLE                     READ_IIC_A1B6_EXTENDED_ADDRESSES
#endif

int main(void)
{

    /* Initialize the device and board peripherals */
    cy_rslt_t result = cybsp_init();

    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }

    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    printf("cy init done\n");

	/* which example should run */
	switch (EXAMPLE) {
		case READ_SPI_SENSOR:
			printf("\n[TLx493D] : compiled example -> read spi sensor\n\n");
			read_spi_sensor();
			break;
		case READ_IIC_SENSOR:
			printf("\n[TLx493D] : compiled example -> read iic sensor\n\n");
			read_iic_sensor();
			break;
		case READ_IIC_SENSOR_PLAIN_C:
			printf("\n[TLx493D] : compiled example -> read iic sensor plain c\n\n");
			read_iic_sensor_plain_c();
			break;
		case READ_IIC_SENSOR_WITH_WAKEUP:
			printf("\n[TLx493D] : compiled example -> read iic sensor with wakeup\n\n");
			read_iic_sensor_with_wakeup();
			break;
		case READ_3_EQUAL_IIC_SENSORS:
			printf("\n[TLx493D] : compiled example -> read 3 equal iic sensors\n\n");
			read_3_equal_iic_sensors();
			break;
		case READ_3_DIFFERENT_IIC_SENSORS:
			printf("\n[TLx493D] : compiled example -> read 3 different iic sensors\n\n");
			read_3_different_iic_sensors();
			break;	
		case READ_IIC_A1B6_EXTENDED_ADDRESSES:
			printf("\n[TLx493D] : compiled example -> read iic a1b6 extended addresses\n\n");
			read_iic_a1b6_extended_addresses();
			break;
	}
}
