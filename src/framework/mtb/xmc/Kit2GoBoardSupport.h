// /*********************************************************************
//  * @file  Kit2GoBoardSupport.h
//  * 
//  * @brief Implementation of the struct Kit2GoBoardSupport for custom sensor/board bring up.
//  * This can be used to power cycle the sensor device, help with setting/resetting the SPI
//  * slave select lines for multiple devices and also setting/resetting addressing lines 
//  * for enabling extended addresses for certain devices.  
//  *********************************************************************/

#ifndef TLX493D_KIT2GO_BOARD_SUPPORT_H
#define TLX493D_KIT2GO_BOARD_SUPPORT_H


// std include
#include<stdint.h>

// XMC related includes
#include "xmc_gpio.h"

/**
* A structure to represent pins with all necessary parameters. 
*/
typedef struct pinCtrl {
        /*@{*/
        bool                        isSet;                          /**< the state of the pin, to be activated at `init` or not. */
        XMC_GPIO_PORT_t*            pinPort;                        /**< the XMC Port of the pin. */
        uint8_t                     pinNumber;                      /**< the XMC Pin number of the pin. */
        XMC_GPIO_MODE_t             direction;                      /**< the mode and direction for the XMC pin. */
        XMC_GPIO_OUTPUT_LEVEL_t     enableValue;                    /**< the logic level of the pin to be enabled or set by the `init` function. */
        XMC_GPIO_OUTPUT_LEVEL_t     disableValue;                   /**< the logic level of the pin when the pin is disabled. */
        uint32_t                    delayAfterEnable;               /**< the delay in Us after the pin is enabled. */
        uint32_t                    delayAfterDisable;              /**< the delay in Us after the pin is disabled. */
        
        /*@}*/
} pinCtrl;


/**
 * @brief The Kit2GoBoardSupport struct used for custom board bring up, for control of SPI slave select lines 
 * and for controlling address lines for extended addressing of certain devices.
 */
typedef struct Kit2GoBoardSupport {
        /**
        * Structure variables to hold parameters of powerPins, selectPins and addrPins.
        */ 
        pinCtrl powerPins[1];
        pinCtrl selectPins[1];
        pinCtrl addrPins[1];
} Kit2GoBoardSupport;

/**
 * @brief The function `initAttributes` initialises datastructures of type PinCtrl to hold parameters for powerPins, selectPins and addrPins.
 * 
 * @param[in] Kit2GoBoardSupport*  Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.     
 */
void bsc_initAttributes(Kit2GoBoardSupport *k2go);

/**
 * @brief The function `init` sets/resets the board pin modes and/or values based on the values of the datastructures set in the main code 
 * and initialises datastructures of type PinCtrl to hold parameters for powerPins, selectPins and addrPins.
 * 
 * @param[in] enablePower Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.
 * @param[in] enableSelect Whether SPI slaves SELECT lines need to be controlled using this class. A particular slave is selected by pulling its 
 * corresponding SELECT line low.
 * @param[in] enableExtendedAddr (Only relevant for Generation 1, A1B6 devices) Whether to use extended addressing feature. In this case, the pin
 *  stored in addrPins structure is activated and later isolated in a prescribed way to enable the 4 extended addresses of the said device.        
 */
void bsc_init(Kit2GoBoardSupport *k2go, bool enablePower, bool enableSelect, bool enableExtendedAddr);

/**
 * @brief The `begin` function recursively calls the `init` function of the same class.
 * 
 * @param[in] enablePower Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.
 * @param[in] enableSelect Whether SPI slaves SELECT lines need to be controlled using this class. A particular slave is selected by pulling its 
 * corresponding SELECT line low.
 * @param[in] enableExtendedAddr (Only relevant for Generation 1, A1B6 devices) Whether to use extended addressing feature. In this case, the pin
 *  stored in addrPins structure is activated and later isolated in a prescribed way to enable the 4 extended addresses of the said device.   
 */
void bsc_begin(Kit2GoBoardSupport *k2go, bool enablePower, bool enableSelect, bool enableExtendedAddr);

/**
 * @brief The `deinit` function sets all the pins to their disableValue.
 */
void bsc_deinit(Kit2GoBoardSupport *k2go);

/**
 * @brief The `end` function recursively calls the `deinit` function of the same class.
*/
void bsc_end(Kit2GoBoardSupport *k2go);
          
/**
 * @brief The `setPowerPin` function is setter function to route pin parameters from the main code into the datastructures/variables of this class.
 * 
 * @param[in] pinNumber Arduino pin number of the GPIO to be used as 3V3 POWER pin for the sensor.
 * @param[in] pinDirection Direction of the Arduino pin to be used as the 3V3 POWER pin.
 * @param[in] pinEnableValue Value of the pin in enabled state.
 * @param[in] pinDisableValue Value of the pin in disabled state.
 * @param[in] delayAfterDisable delay in Us after the pin is disabled. Needed to meet settling time constraints.
 * @param[in] delayAfterEnable delay in Us after the pin is enabled. Needed to meet settling time constraints.  
 */
void bsc_setPowerPin(Kit2GoBoardSupport *k2go, XMC_GPIO_PORT_t * pinPort, uint8_t pinNumber, XMC_GPIO_MODE_t pinDirection, 
XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t delayAfterEnable, uint32_t delayAfterDisable);
/**
 * @brief The `unsetPowerPin` function disables all actions on the pins set in powerPins that would otherwise be performed by the `init` function.
 */
void bsc_unsetPowerPin(Kit2GoBoardSupport *k2go);

/**
 * @brief The `setSelectPin` function is setter function to route pin parameters from the main code into the datastructures/variables of this class.
 * @param[in] pinNumber Arduino pin number of the GPIO to be used as SELECT pin for the sensor used as SPI slave.
 * @param[in] pinDirection Direction of the Arduino pin to be used as the select pin for the sensor used as SPI slave.
 * @param[in] pinEnableValue Value of the pin in enabled state.
 * @param[in] pinDisableValue Value of the pin in disabled state.
 * @param[in] delayAfterDisable delay in Us after the pin is disabled. Needed to meet settling time constraints.
 * @param[in] delayAfterEnable delay in Us after the pin is enabled. Needed to meet settling time constraints.
 */
void bsc_setSelectPin(Kit2GoBoardSupport *k2go, XMC_GPIO_PORT_t * pinPort, uint8_t pinNumber, XMC_GPIO_MODE_t pinDirection, 
        XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t delayAfterEnable, uint32_t delayAfterDisable);

/**
 * @brief The `unsetSelectPin` function disables all actions on the pins set in selectPins that would otherwise be performed by the `init` function.
 */    
void bsc_unsetSelectPin(Kit2GoBoardSupport *k2go);


/**
 * @brief The `controlPower` function sets/resets the pins in powerPins based on argument.
 * 
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */
void bsc_controlPower(Kit2GoBoardSupport *k2go, bool enable);

/**
 * @brief The `controlSelect` function sets/resets the pins in selectPins based on argument.
 * 
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */
void bsc_controlSelect(Kit2GoBoardSupport *k2go, bool enable);

/**
 * @brief The `setAddrPin` function is setter function to route pin parameters from the main code into the datastructures/variables of this class.
 * 
 * @param[in] pinNumber Arduino pin number of the GPIO to be used as the ADDR pin for the sensor used as SPI slave.
 * @param[in] pinDirection Direction of the Arduino pin to be used as the ADDR pin for the sensor used as SPI slave.
 * @param[in] pinEnableValue Value of the pin in enabled state.
 * @param[in] pinDisableValue Value of the pin in disabled state.
 * @param[in] delayAfterDisable delay in Us after the pin is disabled. Needed to meet settling time constraints.
 * @param[in] delayAfterEnable delay in Us after the pin is enabled. Needed to meet settling time constraints.
 */
void bsc_setAddrPin(Kit2GoBoardSupport *k2go, XMC_GPIO_PORT_t * pinPort, uint8_t pinNumber, XMC_GPIO_MODE_t pinDirection, 
        XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t  delayAfterEnable, uint32_t delayAfterDisable);
/**
 * @brief The `unsetAddrPin` function disables all actions on the pins set in addrPins that would otherwise be performed by the `init` function.
 */ 
void bsc_unsetAddrPin(Kit2GoBoardSupport *k2go);

/**
 * @brief The `controlAddr` function sets/resets the pins in addrPins based on argument.
 * 
 * @param enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */    
void bsc_controlAddr(Kit2GoBoardSupport *k2go, bool enable);

void bsc_reset(Kit2GoBoardSupport *k2go);

/**
 * @brief The `initPin` function sets the direction and value of the pin on the basis of parameters in the structure `pinCtrl`.
 * 
 * @param[in] p Structure of type `pinCtrl`.
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */ 
void initPin(pinCtrl *p);

/**
 * @brief The `controlPin` function sets/resets the pin on the basis of parameters in the structure `pinCtrl`.
 * 
 * @param[in] p Structure of type `pinCtrl`.
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */ 
void controlPin(pinCtrl *p, bool enable);


void setPinDirection(pinCtrl *p, XMC_GPIO_MODE_t direction);

#endif // TLX493D_KIT2GO_BOARD_SUPPORT_H
