/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include "button.h"

/***************************************************************************************
* GLOBAL VARIABLES
***************************************************************************************/
bool isButtonPressed = false;
uint32_t buttonLastPressed = 0;
extern fsm_t g_state;

/**
 * @brief       Init all button
 * @param       None
 */
void buttonInit(void){
    pinMode(PIN_BUTTON_CONFIG, INPUT_PULLUP);
    /* set Interrupt for BUTTON_CONFIG */
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_CONFIG), buttonConfigISRHandler, FALLING);
    pinMode(PIN_BUTTON_CONTROL, INPUT);
}
/**
 * @brief       Button IRS Handler
 * @param       None
 */
void buttonConfigISRHandler(void){
    if(STATE_CONFIG != g_state){
        isButtonPressed = true;
        buttonLastPressed = millis();
    }
}
/**
 * @brief       Button config check
 * @param       None
 * @retval      true
 *              false
 */
bool buttonConfigCheck(void){
    if(isButtonPressed = true){
        // Check if button has been pressed for 3s
        if(digitalRead(PIN_BUTTON_CONFIG) == LOW){
            if((millis() - buttonLastPressed) > 3000){
                if(digitalRead(PIN_BUTTON_CONFIG) == LOW){
                  isButtonPressed = false;
                  return true;
                }
            }
        }
    }
    return false;
}
/**
 * @brief       Button config check
 * @param       None
 * @retval      true
 *              false
 */
bool buttonDeviceCheck(void)
{
    static uint8_t _last_status = HIGH;
    static uint32_t _last_time = 0;
    if(digitalRead(PIN_BUTTON_CONTROL) != _last_status){
        _last_time = millis();
    }
    if((millis() - _last_status) > 300)
    {
        if(digitalRead(PIN_BUTTON_CONTROL) != _last_status){
            _last_status = digitalRead(PIN_BUTTON_CONTROL);
            return true;
        }
    }
    return false;
}