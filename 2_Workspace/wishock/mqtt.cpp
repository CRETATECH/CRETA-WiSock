
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
char gMqttTopicIn[25];
char gMqttTopicOut[25];
WiFiClient gESPClient;
PubSubClient client(gESPClient);
const char* gMqttServer = "iot.eclipse.org";
/*************************************************/
/*                  LOCAL  VARIABLE              */
/*************************************************/
char gClientID[25];

/*************************************************/
/*                  FUCTION PROTOTYPE            */
/*************************************************/
String Get_macID (void);
void mqttCreateClientID (void);
/*************************************************/
/*                  MAIN FUNCTION                */
/*************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  String _mqttRecvData = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    _mqttRecvData += String((char)payload[i]);
  }
  Serial.println(_mqttRecvData);
  Serial.println();
  protocolDataProcess(_mqttRecvData);
}

void mqttCreateTopic(void)
{
  String _nameTopic = "ESP" + Get_macID();
  
  String _topic = _nameTopic + "/master";
  _topic.toCharArray(gMqttTopicIn, 24);
  
  _topic = _nameTopic + "/slave";
  _topic.toCharArray(gMqttTopicOut, 23);
  
  Serial.print("topicIn: ");
  Serial.println(gMqttTopicIn);
  Serial.print("topicOut: ");
  Serial.println(gMqttTopicOut);
  
  mqttCreateClientID ();
  client.setServer(gMqttServer, 1883);
  client.setCallback(callback);
}

void mqttSubscribe(void)
{
   client.subscribe(gMqttTopicIn, 0);
   Serial.println("subscribe"); 
}

int mqttConnect (void)
{
  return client.connect(gClientID);
}

int mqttConnected (void)
{
  return client.connected();
}

void mqttPublish (String pJsonOut)
{
  char _dataOut[100];
  pJsonOut.toCharArray(_dataOut, pJsonOut.length() + 1);
  client.publish(gMqttTopicOut, _dataOut);
  Serial.print("Publish json: ");
  Serial.print(_dataOut);
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
  String _val;
  byte _mac[6];
  WiFi.macAddress(_mac);
  for (int i = 0; i < 6; i++)
  {
    if (_mac[i] < 0x10)
      _val += '0' + String(_mac[i], HEX);
    else _val += String(_mac[i], HEX);
  }
  return _val;
}

void mqttCreateClientID (void)
{
  String _temp = Get_macID();
  _temp = "ESP" + _temp;
  _temp.toCharArray(gClientID, 25);
}

