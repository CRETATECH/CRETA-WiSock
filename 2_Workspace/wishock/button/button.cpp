#include <ESP8266WiFi.h>

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