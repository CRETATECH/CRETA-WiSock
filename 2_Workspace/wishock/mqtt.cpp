
/*************************************************/
/*                  INCLUDE                      */
/*************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "wishock.h"

/*************************************************/
/*                  GLOBAL VARIABLE              */
/*************************************************/
extern char topicIn[25];
extern char topicOut[25];
extern WiFiClient espClient;
extern PubSubClient client;
/*************************************************/
/*                  FUCTION PROTOTYPE            */
/*************************************************/
String Get_macID (void);

/*************************************************/
/*                  MAIN FUNCTION                */
/*************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqttCreateTopic(void)
{
  String nameTopic = "ESP" + Get_macID();
  String vTopic = nameTopic + "/master";
  vTopic.toCharArray(topicIn, 24);
  vTopic = nameTopic + "/slave";
  vTopic.toCharArray(topicOut, 23);
  Serial.print("topicIn: ");
  Serial.println(topicIn);
  Serial.print("topicOut: ");
  Serial.println(topicOut);
}

void mqttSubscribe(void)
{
   client.subscribe(topicIn, 0); 
}

int mqttConnect (void)
{
  return client.connect("wishock");
}

int mqttConnected (void)
{
  return client.connected();
}

/*************************************************/
/*                  LOCAL FUCTION                */
/*************************************************/
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


