/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wishock.h"
#include "button.h"
#include "device.h"
#include "protocol.h"
#include "mqtt.h"
/***************************************************************************************
* EXTERN VARIABLES
***************************************************************************************/
String gID = "";
String gFunc = "";
String gAddr ="";
String gData = "";
/***************************************************************************************
* LOCAL FUNCTIONS PROTOTYPES
***************************************************************************************/
int protocolCtrlFunc_Process (void);
int protocolDataFunc_Process (void);
/***************************************************************************************
* PUBLIC FUNCTION
***************************************************************************************/
void protocolInit(void)
{


}

int jsonParse(String pJson)
{
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& root = _jsonBuffer.parseObject(pJson);
  if (!root.success())
  {
    Serial.println("pars fail");
    return 0;
  } 
  String _a      = root["ID"];
  gID = _a;
  String _b      = root["FUNC"];
  gFunc = _b;
  String _c      = root["ADDR"];
  gAddr = _c;
  String _d      = root["DATA"];
  gData = _d;
      
  Serial.println("pars ok");
  return 1; 
}

String protocolCreateJson (String pFunc, String pAddr, String pData)
{
  
   String _stringout = "{\"ID\" : \"ESP"  + Get_macID() + "\", \"FUNC\" : \"" + pFunc + "\", \"ADDR\" : \"" + pAddr + "\", \"DATA\" : \"" + pData + "\"}";
   return _stringout;
}

void protocolDataProcess (String pJsonRecv)
{
  int _temp;
  /* parse json */
  if (jsonParse(pJsonRecv) == 0)
  {
    mqttPublish(protocolCreateJson("Error", "1", "ErrFrame"));  
  }
  else
  {
    /* process */
    if (gFunc == "Ctrl")
    {
      Serial.println("func ctrl");
      _temp = protocolCtrlFunc_Process();
      if (_temp == PROCESS_NORMAL)
      {
        mqttPublish(protocolCreateJson("Ctrl", "1", gData)); 
      }
      else if (_temp == PROCESS_ERR)
      {
        mqttPublish(protocolCreateJson("Ctrl", "1", "ErrProcess"));   
      }
      else
        mqttPublish(protocolCreateJson("Ctrl", "1", "ErrFrame"));
    }
    else if (gFunc == "Data")
    {
      Serial.println("func data");
      _temp  = protocolDataFunc_Process();
      if (_temp == PROCESS_NORMAL)
      {
        mqttPublish(protocolCreateJson("Data", "1", gData)); 
      }
      else
        mqttPublish(protocolCreateJson("Ctrl", "1", "ErrFrame"));
    }
    else
    {
      mqttPublish(protocolCreateJson("Error", "1", "ErrFrame"));    
    }
  }
}

int protocolCtrlFunc_Process (void)
{
  if (gAddr == "1")
  {
    if (gData == "On")
    {
      deviceOn();
      delay(50);
      if (deviceStatus() == DEVICE_ON)
        return PROCESS_NORMAL;
      else return PROCESS_ERR;
    }
    else if (gData == "Off")
    {
      deviceOff();
      delay(50);
      if (deviceStatus() == DEVICE_OFF)
        return PROCESS_NORMAL;
      else return PROCESS_ERR;
    }
    else return FRAME_ERR;
  }
  else return FRAME_ERR;
}

int protocolDataFunc_Process (void)
{
  if (gAddr == "1")
  {
    if (deviceStatus() == DEVICE_ON)
      gData = "On";
    else
      gData = "Off";
    return PROCESS_NORMAL; 
  }
  else
    return FRAME_ERR;
}


