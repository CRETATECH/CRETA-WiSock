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



/*************************************************/
/*                  MAIN FUCTION                 */
/*************************************************/
void setup() {
  int g_flag_config = 0;
  System_Init();
  Topic_Set();
  /* check that has Wishock configed for the first time yet? */
  /* 0x05: configed, another: not confied yet */
  if (EEPROM_Read() == 0x05)
    g_flag_config = 1; // configed
  else g_flag_config = 0; // not configed yet

  if (g_flag_config == 0)
    g_state = STATE_CONFIG;
  else g_state = STATE_CONTROL;
}

void loop() {
  switch(g_state)
  {
    case STATE_CONFIG:   func_state_config();  break;
    case STATE_CONTROL:  func_state_control(); break;
    default:                                   break;
  }

}

/*************************************************/
/*                  LOCAL FUCTION                */
/*************************************************/

void System_Init (void)
{
  Serial.begin(115200);
  pinMode(BUTTON_CONFIG, INPUT_PULLUP);
  /* set Interrupt for BUTTON_CONFIG */
  attachInterrupt(digitalPinToInterrupt(BUTTON_CONFIG), ConfigButton_ISR, FALLING);
  pinMode(BUTTON_CONTROL, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(DEVICE, OUTPUT);

  /* Init timer */
  os_timer_disarm(&Timer);
  os_timer_setfn(&Timer, (os_timer_func_t *)Timer_ISR, NULL);
  os_timer_arm(&Timer, 500, 1);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void func_state_config(void)
{
  WiFi.beginSmartConfig();
  while(1)
  {
    delay(1000);
    if (WiFi.smartConfigDone())
      break;
  }
  /* write 0x05 to address 0x10 to inform that configed */
  EEPROM_Write (0x05);
  g_state = STATE_CONTROL;
}

void func_state_control (void)
{
  
}

void ConfigButton_ISR (void)
{
  
}

String Get_macID (void)
{
  String val;
  byte mac[6];
  WiFi.macAddress(mac);
  val = String(mac[0], HEX) + String(mac[1], HEX) + String(mac[2], HEX) + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);
  return val;
}

void Timer_ISR (void)
{
  
}

void Wifi_Connect (void)
{
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef DEBUG
      Serial.print(".");
    #endif

    delay(100);
  }
}

//int pars_json(String json)
//{
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject& root = jsonBuffer.parseObject(json);
//  if (!root.success())
//  {
//    return 0;
//  }
//  g_data.ID       = root["ID"];
//  g_data.func     = root["FUNC"];
//  g_data.addr     = root["ADDR"];
//  g_data.data     = root["DATA"];
//  return 1; 
//}

void EEPROM_Write (unsigned char value)
{
  EEPROM.begin(50);
  EEPROM.write(EEPROM_ADDRESS, value);
  EEPROM.commit();
}

byte EEPROM_Read (void)
{
  EEPROM.begin(50);
  return EEPROM.read(EEPROM_ADDRESS);
}

void Topic_Set (void)
{
  String nameTopic = "ESP" + Get_macID();
  topicIn  = nameTopic + "/master";
  topicOut = nameTopic + "/slave"; 
}


