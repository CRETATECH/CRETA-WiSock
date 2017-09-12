/*
 * Wishock.ino
 * Date: 10/9/2017
 */

/*************************************************/
/*                  INCLUDE                      */
/*************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "user_interface.h"
#include "wishock.h"
#include "state.h"
/*************************************************/
/*                  GLOBAL VARIABLE              */
/*************************************************/


//WiFiClient espClient;
//PubSubClient client(espClient);



//led_status_t LED_FLAG = LED_STATUS_BLINK;

//const char* mqtt_server = "iot.eclipse.org";

//char topicIn[25];
//char topicOut[25];
/*************************************************/
/*                  MAIN FUCTION                 */
/*************************************************/
void setup() {
  stateSetup();
}


void loop() {
  stateUpdate();
  stateMachine();

}






