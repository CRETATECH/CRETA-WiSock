
/*************************************************/
/*                  INCLUDE                      */
/*************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "wishock.h"
#include "protocol.h"
#include "button.h"
#include "device.h"
/*************************************************/
/*                  EXTERN VARIABLE              */
/*************************************************/
char topicIn[25];
char topicOut[25];
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "iot.eclipse.org";
/*************************************************/
/*                  LOCAL  VARIABLE              */
/*************************************************/
char clientID[25];

/*************************************************/
/*                  FUCTION PROTOTYPE            */
/*************************************************/
String Get_macID (void);
void mqttCreateClientID (void);
/*************************************************/
/*                  MAIN FUNCTION                */
/*************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  String mqtt_recvdata = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    mqtt_recvdata += String((char)payload[i]);
  }
  Serial.println(mqtt_recvdata);
  Serial.println();
  DataProcess(mqtt_recvdata);
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
  mqttCreateClientID ();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void mqttSubscribe(void)
{
   client.subscribe(topicIn, 0);
   Serial.println("subscribe"); 
}

int mqttConnect (void)
{
  return client.connect(clientID);
}

int mqttConnected (void)
{
  return client.connected();
}

void mqttPublish (String jsonOut)
{
  char dataOut[100];
  jsonOut.toCharArray(dataOut,jsonOut.length() + 1);
  client.publish(topicOut, dataOut);
  Serial.print("Publish json: ");
  Serial.print(dataOut);
}

void mqttLoop (void)
{
  client.loop();
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

void mqttCreateClientID (void)
{
  String temp = Get_macID();
  temp = "ESP" + temp;
  temp.toCharArray(clientID, 25);
}

