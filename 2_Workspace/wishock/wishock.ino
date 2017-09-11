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
  if (EEPROM_Read_ConfigFlag() == 0x05)
    g_flag_config = 1; // configed
  else g_flag_config = 0; // not configed yet

  if (g_flag_config == 0)
    g_state = STATE_CONFIG;
  else g_state = STATE_CONTROL;

    client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {
  switch(g_state)
  {
    case STATE_CONFIG:   func_config();  break;
    case STATE_CONTROL:  func_control(); break;
    default:                                   break;
  }

}

/*************************************************/
/*                  LOCAL FUCTION                */
/*************************************************/

void System_Init (void)
{
  Serial.begin(115200);
//  pinMode(PIN_BUTTON_CONFIG, INPUT_PULLUP);
//  /* set Interrupt for BUTTON_CONFIG */
//  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_CONFIG), ConfigButton_ISR, FALLING);
//  pinMode(PIN_BUTTON_CONTROL, INPUT);
//  pinMode(PIN_LED, OUTPUT);
//  pinMode(PIN_DEVICE, OUTPUT);

  /* Init timer */
  os_timer_disarm(&Timer);
  os_timer_setfn(&Timer, (os_timer_func_t *)Timer_ISR, NULL);
  os_timer_arm(&Timer, 200, 1);
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

void func_config(void)
{
  Serial.println("Start config..."); 
  LED_FLAG = LED_STATUS_OFF;
  WiFi.beginSmartConfig();
  while(1)
  {
    delay(1000);
    if (WiFi.smartConfigDone())
      break;
  }
  /* write 0x05 to address 0x10 to inform that configed */
  EEPROM_Write_ConfigFlag (0x05);
  g_state = STATE_CONTROL;
  if (EEPROM_Read_ConfigFlag() == 0x05)
    Serial.println("Flag saved!!!");
  Serial.println("Config done!!!");   
}

void func_control (void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      //reconnect();
      if (client.connect("ESP8266"))
      {
        Serial.println("sub");
        client.subscribe(topicIn, 0);
      }
    }
    if (client.connected())
    {
      client.loop();
      Serial.println("loop");
    }
  }
  else Wifi_Connect();
  delay(1000);
  //client.publish(topicOut, "hello world");
  g_state = STATE_CONTROL;
}

//void reconnect(void) {
//  // Loop until we're reconnected
//  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");
//    // Attempt to connect
//    if (client.connect("ESP8266Client")) {
//      Serial.println("connected");
//      // Once connected, publish an announcement...
//      //client.publish(topicOut, "hello world");
//      // ... and resubscribe
//      client.subscribe(topicIn);
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(2000);
//    }
//  }
//}

void ConfigButton_ISR (void)
{
  static uint32_t _last_pulse = 0;
  static uint32_t _button_pressed = 1;
  if(0 == _last_pulse)
    _last_pulse = millis();
  if(STATE_CONFIG != g_state)
  {
    if((millis() - _last_pulse) > 3000)
    {
      if(_button_pressed == 0)
        _button_pressed = 1;
      else
      {
        _button_pressed = 0;
        g_state = STATE_CONFIG;
      }
    }
  }
  _last_pulse = millis();
}

String Get_macID (void)
{
  String val;
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i = 0; i < 6; i++)
  {
    if (mac[i] < 0x10)
      val += '0' + String(mac[i], HEX);
    else val += String(mac[i], HEX);
  }
  return val;
}

void Timer_ISR (void)
{

  if (LED_FLAG == LED_STATUS_BLINK)
  {
     int temp = digitalRead(PIN_LED);
     digitalWrite(PIN_LED, !temp);   
  }
  else if (LED_FLAG == LED_STATUS_ON)
    Led_On();
  else Led_Off();
}

void Wifi_Connect (void)
{
  Serial.println("bat dau ket noi wifi");
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef DEBUG
      Serial.print(".");
    #endif

    delay(100);
  }

}

int pars_json(String json)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success())
  {
    Serial.println("pars fail");
    return 0;
  }
  String a     = root["ID"];
  String b     = root["FUNC"];
  //String c     = root["ADDR"];
  String d     = root["DATA"];
  Serial.println("pars ok");
  return 1; 
}

void EEPROM_Write_ConfigFlag (unsigned char value)
{
  EEPROM.begin(50);
  EEPROM.write(EEPROM_ADDRESS, value);
  EEPROM.commit();
}

byte EEPROM_Read_ConfigFlag (void)
{
  EEPROM.begin(50);
  return EEPROM.read(EEPROM_ADDRESS);
}

void Topic_Set (void)
{
  String nameTopic = "ESP" + Get_macID();
  String vTopic = nameTopic + "/master";
  vTopic.toCharArray(topicIn, 24);
  vTopic = nameTopic + "/slave";
  vTopic.toCharArray(topicOut, 23);     

}

void Led_On (void)
{
  digitalWrite(PIN_LED, HIGH);
}

void Led_Off (void)
{
  digitalWrite (PIN_LED, LOW);
}

