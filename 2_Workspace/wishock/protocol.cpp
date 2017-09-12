
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
void protocolInit(void)
{


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
  String c     = root["ADDR"];
  String d     = root["DATA"];
  Serial.println("pars ok");
  return 1; 
}
