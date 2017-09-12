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

void deviceOn(void){
    digitalWrite(PIN_DEVICE, LOW);
}

void deviceOff(void){
    digitalWrite(PIN_DEVICE, HIGH);
}

void ledOn(void){
    digitalWrite(PIN_LED, LOW);
}

void ledOff(void){
    digitalWrite(PIN_LED, HIGH);
}

void deviceToggle(void){
    if(digitalRead(PIN_DEVICE) == HIGH)
        deviceOn();
    else
        deviceOff();
}

void ledToggle(void){
    if(digitalRead(PIN_LED) == HIGH)
        deviceOn();
    else
        deviceOff();
}
