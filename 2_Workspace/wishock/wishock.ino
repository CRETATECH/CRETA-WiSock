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
os_timer_t Timer;

WiFiClient espClient;
PubSubClient client(espClient);
fsm_t g_state = STATE_CONTROL;


led_status_t LED_FLAG = LED_STATUS_BLINK;

const char* mqtt_server = "iot.eclipse.org";

char topicIn[25];
char topicOut[25];
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

/*************************************************/
/*                  LOCAL FUCTION                */
/*************************************************/




//void ConfigButton_ISR (void)
//{
//  static uint32_t _last_pulse = 0;
//  static uint32_t _button_pressed = 1;
//  if(0 == _last_pulse)
//    _last_pulse = millis();
//  if(STATE_CONFIG != g_state)
//  {
//    if((millis() - _last_pulse) > 3000)
//    {
//      if(_button_pressed == 0)
//        _button_pressed = 1;
//      else
//      {
//        _button_pressed = 0;
//        g_state = STATE_CONFIG;
//      }
//    }
//  }
//  _last_pulse = millis();
//}

//}


