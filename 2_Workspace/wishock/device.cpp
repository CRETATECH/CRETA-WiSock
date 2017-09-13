/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include "device.h"
#include <ESP8266WiFi.h>

/***************************************************************************************
* PRIVATE DEFINES
***************************************************************************************/

/***************************************************************************************
* PUBLIC VARIABLES
***************************************************************************************/

/***************************************************************************************
* PUBLIC FUNCTIONS
***************************************************************************************/
void deviceInit(void){
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_DEVICE, OUTPUT);
}
/**
 * Turn device ON
 */
void deviceOn(void){
    digitalWrite(PIN_DEVICE, LOW);
    Serial.println("device on");
}
/**
 * Turn device OFF
 */
void deviceOff(void){
    digitalWrite(PIN_DEVICE, HIGH);
    Serial.println("device off");
}
/**
 * Turn led ON
 */
void ledOn(void){
    digitalWrite(PIN_LED, LOW);
}
/**
 * Turn led OFF
 */
void ledOff(void){
    digitalWrite(PIN_LED, HIGH);
}
/**
 * Toggle device
 */
void deviceToggle(void){
    if(digitalRead(PIN_DEVICE) == HIGH)
        deviceOn();
    else
        deviceOff();
}
/**
 * Toggle led
 */
void ledToggle(void){
    if(digitalRead(PIN_LED) == HIGH)
        ledOn();
    else
        ledOff();
}
/**
 * @brief       Return device status
 * @retval      HIGH
 *              LOW
 */
int deviceStatus(void){
    return digitalRead(PIN_DEVICE);
}
