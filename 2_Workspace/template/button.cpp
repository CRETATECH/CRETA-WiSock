/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include "button.h"


/***************************************************************************************
* PRIVATE DEFINES
***************************************************************************************/
// #define DEBUG

/***************************************************************************************
* PUBLIC VARIABLES
***************************************************************************************/
bool isButtonPressed = false;

/***************************************************************************************
* PUBLIC FUNCTIONS
***************************************************************************************/
/**
 * @brief       Init all button
 * @param       None
 */
void buttonInit(void){
    pinMode(PIN_BUTTON_CONFIG, INPUT_PULLUP);
#ifdef DEBUG
    pinMode(PIN_BUTTON_CONTROL, INPUT_PULLUP);
#else
    pinMode(PIN_BUTTON_CONTROL, INPUT);
#endif
    /* set Interrupt for BUTTON_CONTROL */
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_CONTROL), buttonConfigISRHandler, CHANGE);
}
/**
 * @brief       Button IRS Handler
 * @param       None
 */
void buttonConfigISRHandler(void){
    static uint32_t _last_pulse = 0;
    if((millis() - _last_pulse) > 200){
        isButtonPressed = true;
    }
    _last_pulse = millis();
}
/**
 * @brief       Button config check
 * @param       None
 * @retval      true
 *              false
 */
bool buttonConfigCheck(void){
    static uint32_t _last_pressed = 0;
    static uint8_t _last_status = HIGH;
    static uint8_t _current_status;
    _current_status = digitalRead(PIN_BUTTON_CONFIG);
    if(LOW == _current_status){
        if(_current_status != _last_status){
            _last_pressed = millis();
        } else{
            if((millis() - _last_pressed) > 3000){
                return true;
            }
        }
    }
    _last_status = _current_status;
    return false;
}
/**
 * @brief       Button config check
 * @param       None
 * @retval      true
 *              false
 */
bool buttonDeviceCheck(void){
    if(true == isButtonPressed){
        isButtonPressed = false;
        return true;
    } else{
        return false;
    }
}
