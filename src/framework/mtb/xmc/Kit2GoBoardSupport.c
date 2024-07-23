// /*********************************************************************
//  * @file  Kit2GoBoardSupport.c
//  * 
//  * @brief Implementation of the struct Kit2GoBoardSupport for custom sensor/board bring up.
//  * This can be used to power cycle the sensor device, help with setting/resetting the SPI
//  * slave select lines for multiple devices and also setting/resetting addressing lines 
//  * for enabling extended addresses for certain devices.  
//  *********************************************************************/

#ifndef TLX493D_KIT2GO_BOARD_SUPPORT_C
#define TLX493D_KIT2GO_BOARD_SUPPORT_C


// std include
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stddef.h>

// XMC related includes
#include "xmc_gpio.h"

// project c includes
#include "Kit2GoBoardSupport.h"

// todo: the order might differ from Arudino. But there are compilation warnings about implicit declaration otherwise



/**
 * @brief The function `initAttributes` initialises datastructures of type PinCtrl to hold parameters for powerPins, selectPins and addrPins.
 * 
 * @param[in] Kit2GoBoardSupport*  Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.     
 */
void bsc_initAttributes(Kit2GoBoardSupport *k2go) {

        // function calls to set initialization/default values to struct members
        // the unset() method is called to set the isSet member to the value FALSE
        // Note: for XMC, the PORT0 has to be the init PORT address. Any other init value puts the 
        // device in unknown state.
        bsc_setAddrPin(k2go, XMC_GPIO_PORT0, 0, 0, 0, 0, 0, 0);
        bsc_unsetAddrPin(k2go);
        
        bsc_setPowerPin(k2go, XMC_GPIO_PORT0, 0, 0, 0, 0, 0, 0);
        bsc_unsetPowerPin(k2go);

        bsc_setSelectPin(k2go, XMC_GPIO_PORT0, 0, 0, 0, 0, 0, 0);
        bsc_unsetSelectPin(k2go);
}

/**
 * @brief The function `init` sets/resets the board pin modes and/or values based on the values of the datastructures set in the main code
 * 
 * @param[in] enablePower Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.
 * @param[in] enableSelect Whether SPI slaves SELECT lines need to be controlled using this class. A particular slave is selected by pulling its 
 * corresponding SELECT line low.
 * @param[in] enableExtendedAddr (Only relevant for Generation 1, A1B6 devices) Whether to use extended addressing feature. In this case, the pin
 *  stored in addrPins structure is activated and later isolated in a prescribed way to enable the 4 extended addresses of the said device.        
 */

void bsc_init(Kit2GoBoardSupport *k2go, bool enablePower, bool enableSelect, bool enableExtendedAddr) {

        for(size_t i=0; i<sizeof(k2go->selectPins)/sizeof(pinCtrl); i++) {
                initPin(&k2go->selectPins[i]);
        }

        bsc_controlSelect(k2go, enableSelect);

        
        if(enableExtendedAddr){
                for(size_t i=0; i<sizeof(k2go->addrPins)/sizeof(pinCtrl); i++){  
                        initPin(&k2go->addrPins[i]);
                }
                bsc_controlAddr(k2go, true);
        }

        for(size_t i=0; i<sizeof(k2go->powerPins)/sizeof(pinCtrl); i++) {
                initPin(&k2go->powerPins[i]);
        }

        bsc_controlPower(k2go, false);

        if(enablePower){
                bsc_controlPower(k2go, true);
        }

        if(enableExtendedAddr){
                bsc_controlAddr(k2go, false);

                for(size_t i=0; i< sizeof(k2go->addrPins)/sizeof(pinCtrl); i++){
                        setPinDirection(&k2go->addrPins[i], XMC_GPIO_MODE_INPUT_TRISTATE);
                }
        }
}

/**
 * @brief The `begin` function recursively calls the `init` function of the same class.
 * 
 * @param[in] enablePower Whether to power up the board using this class. In this case the board 3V3 is routed through a GPIO of the host MCU.
 * @param[in] enableSelect Whether SPI slaves SELECT lines need to be controlled using this class. A particular slave is selected by pulling its 
 * corresponding SELECT line low.
 * @param[in] enableExtendedAddr (Only relevant for Generation 1, A1B6 devices) Whether to use extended addressing feature. In this case, the pin
 *  stored in addrPins structure is activated and later isolated in a prescribed way to enable the 4 extended addresses of the said device.   
 */
void bsc_begin(Kit2GoBoardSupport *k2go, bool enablePower, bool enableSelect, bool enableExtendedAddr) {
        bsc_init(k2go, enablePower, enableSelect, enableExtendedAddr);
}

/**
 * @brief The `deinit` function sets all the pins to their disableValue.
 */
void bsc_deinit(Kit2GoBoardSupport *k2go) {
        bsc_controlPower(k2go, false);

        bsc_controlSelect(k2go, false);
}

/**
 * @brief The `end` function recursively calls the `deinit` function of the same class.
*/
void bsc_end(Kit2GoBoardSupport *k2go) {
        bsc_deinit(k2go);
}

             
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
    XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t delayAfterEnable, uint32_t delayAfterDisable) {
        k2go->powerPins[0] = (pinCtrl) { true, pinPort, pinNumber, pinDirection, pinEnableValue, pinDisableValue, delayAfterEnable, delayAfterDisable  };
}

/**
 * @brief The `unsetPowerPin` function disables all actions on the pins set in powerPins that would otherwise be performed by the `init` function.
 */
void bsc_unsetPowerPin(Kit2GoBoardSupport *k2go) {
        k2go->powerPins[0].isSet = false;
}

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
    XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t delayAfterEnable, uint32_t delayAfterDisable) {
        k2go->selectPins[0] = (pinCtrl) { true, pinPort, pinNumber, pinDirection, pinEnableValue, pinDisableValue, delayAfterEnable, delayAfterDisable  };
}

/**
 * @brief The `unsetSelectPin` function disables all actions on the pins set in selectPins that would otherwise be performed by the `init` function.
 */    
void bsc_unsetSelectPin(Kit2GoBoardSupport *k2go) {
        k2go->selectPins[0].isSet = false;
}


/**
 * @brief The `controlPower` function sets/resets the pins in powerPins based on argument.
 * 
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */
void bsc_controlPower(Kit2GoBoardSupport *k2go, bool enable) {
        for(size_t i=0; i<sizeof(k2go->powerPins)/sizeof(pinCtrl); i++) {
                controlPin(&k2go->powerPins[i], enable);
        }
}

/**
 * @brief The `controlSelect` function sets/resets the pins in selectPins based on argument.
 * 
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */
void bsc_controlSelect(Kit2GoBoardSupport *k2go, bool enable) {
        for(size_t i=0; i<sizeof(k2go->selectPins)/sizeof(pinCtrl); i++) {
                controlPin(&k2go->selectPins[i], enable);
        }
}

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
    XMC_GPIO_OUTPUT_LEVEL_t pinEnableValue, XMC_GPIO_OUTPUT_LEVEL_t pinDisableValue, uint32_t  delayAfterEnable, uint32_t delayAfterDisable) {
        k2go->addrPins[0] = (pinCtrl) { true, pinPort, pinNumber, pinDirection, pinEnableValue, pinDisableValue, delayAfterEnable, delayAfterDisable};
}

/**
 * @brief The `unsetAddrPin` function disables all actions on the pins set in addrPins that would otherwise be performed by the `init` function.
 */ 
void bsc_unsetAddrPin(Kit2GoBoardSupport *k2go) {
        k2go->addrPins[0].isSet = false;
}

/**
 * @brief The `controlAddr` function sets/resets the pins in addrPins based on argument.
 * 
 * @param enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */    
void bsc_controlAddr(Kit2GoBoardSupport *k2go, bool enable) {
        for(size_t i=0; i<sizeof(k2go->addrPins)/sizeof(pinCtrl); i++) {
                controlPin(&k2go->addrPins[i], enable);
        }
}

void bsc_reset(Kit2GoBoardSupport *k2go){
        bsc_controlPower(k2go, false);
        bsc_controlPower(k2go, true);        
}


/**
 * @brief The `controlPin` function sets/resets the pin on the basis of parameters in the structure `pinCtrl`.
 * 
 * @param[in] p Structure of type `pinCtrl`.
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */ 
void controlPin(pinCtrl *p, bool enable) {
        if( p->isSet ) {
                XMC_GPIO_SetOutputLevel( p->pinPort, p->pinNumber,  enable ? p->enableValue : p->disableValue );
                XMC_DelayUs(enable ? p->delayAfterEnable : p->delayAfterDisable);
        }
}

/**
 * @brief The `initPin` function sets the direction and value of the pin on the basis of parameters in the structure `pinCtrl`.
 * 
 * @param[in] p Structure of type `pinCtrl`.
 * @param[in] enable Sets pin to pinEnableValue if true, else to pinDisableValue.
 */ 
void initPin(pinCtrl *p) {
       const XMC_GPIO_CONFIG_t gpio_config = {.mode = p->direction, .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
#if UC_FAMILY == XMC1
	.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
#endif
        };
	XMC_GPIO_Init( p->pinPort, p->pinNumber, &gpio_config );
}

void setPinDirection(pinCtrl *p, XMC_GPIO_MODE_t direction){
        if(p->isSet){
                p->direction = direction;
                const XMC_GPIO_CONFIG_t gpio_config = {.mode = direction, .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,  
#if UC_FAMILY == XMC1
                        .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
#endif
                        };
	        XMC_GPIO_Init(p->pinPort, p->pinNumber, &gpio_config);   

        }
}


#endif // TLX493D_KIT2GO_BOARD_SUPPORT_C
