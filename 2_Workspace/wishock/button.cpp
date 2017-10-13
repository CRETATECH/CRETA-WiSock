/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include "button.h"
#include "device.h"
#include "mqtt.h"
#include "protocol.h"

/***************************************************************************************
* LOCAL VARIABLES
***************************************************************************************/

/***************************************************************************************
* EXTERN VARIABLES
***************************************************************************************/
bool isButtonPressed = false;
/***************************************************************************************
* PUBLIC FUNCTION
***************************************************************************************/
/**
 * @brief       Init all button
 * @param       None
 */
void buttonInit(void){
    pinMode(PIN_BUTTON_CONFIG, INPUT_PULLUP);
    /* set Interrupt for BUTTON_CONFIG */
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_CONTROL), buttonConfigISRHandler, CHANGE);
    pinMode(PIN_BUTTON_CONTROL, INPUT);
}
/**
 * @brief       Button IRS Handler
 * @param       None
 */
void buttonConfigISRHandler(void){
    static uint32_t _button_last_pressed = 0;
    if((millis() - _button_last_pressed) > 400){
        isButtonPressed = true;
        
    }
    _button_last_pressed = millis();
}
/**
 * @brief       Button config check
 * @param       None
 * @retval      true
 *              false
 */
bool buttonConfigCheck(void){
    static uint8_t buttonLastStatus = HIGH;
    static uint32_t buttonLastPressed = 0;
    uint8_t buttonStatus = digitalRead(PIN_BUTTON_CONFIG);
    if(buttonStatus != buttonLastStatus){
        if(buttonStatus == LOW){
            buttonLastPressed = millis();
        }
    }
    else{
        if(buttonStatus == LOW){
            if((millis() - buttonLastPressed) > 3000){
                return true;
            }
        }
    }
    buttonLastStatus = buttonStatus;
    return false;
    
    
}

bool buttonControlCheck(void){
    if(isButtonPressed == true){
        isButtonPressed = false;
        return true;
    }
    return false;
}

