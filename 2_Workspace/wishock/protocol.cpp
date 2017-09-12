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
String ID = "";
String func = "";
String addr ="";
String data = "";
/***************************************************************************************
* LOCAL FUNCTIONS PROTOTYPES
***************************************************************************************/
int CtrlFunc_Process (void);
int DataFunc_Process (void);
/***************************************************************************************
* PUBLIC FUNCTION
***************************************************************************************/
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
  String a      = root["ID"];
  ID = a;
  String b      = root["FUNC"];
  func = b;
  String c      = root["ADDR"];
  addr = c;
  String d      = root["DATA"];
  data = d;
      
  Serial.println("pars ok");
  return 1; 
}

String CreateJson (String pFunc, String pAddr, String pData)
{
  
   String a = "{\"ID\" : \"" + Get_macID() + "\", \"FUNC\" : \"" + pFunc + "\", \"ADDR\" : \"" + pAddr + "\", \"DATA\" : \"" + pData + "\"}";
   return a;
}

void DataProcess (String recv_json)
{
  int temp;
  /* parse json */
  pars_json(recv_json);
  /* process */
  if (func == "Ctrl")
  {
    temp = CtrlFunc_Process();
    if (temp == PROCESS_NORMAL)
    {
      mqttPublish(CreateJson("Ctrl", "1", data)); 
    }
    else if (temp == PROCESS_ERR)
    {
      mqttPublish(CreateJson("Ctrl", "1", "ErrProcess"));   
    }
    else
      mqttPublish(CreateJson("Ctrl", "1", "ErrFrame"));
  }
  else if (func == "Data")
  {
    temp  = DataFunc_Process();
    if (temp == PROCESS_NORMAL)
    {
      mqttPublish(CreateJson("Data", "1", data)); 
    }
    else
      mqttPublish(CreateJson("Ctrl", "1", "ErrFrame"));
  }
  else
  {
    mqttPublish(CreateJson("Ctrl", "1", "ErrFrame"));    
  }
}

int CtrlFunc_Process (void)
{
  if (addr == "1")
  {
    if (data == "On")
    {
      deviceOn();
      delay(50);
      if (deviceStatus() == DEVICE_ON)
        return PROCESS_NORMAL;
      else return PROCESS_ERR;
    }
    else if (data == "Off")
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

int DataFunc_Process (void)
{
  if (data == "1")
  {
    if (deviceStatus() == DEVICE_ON)
      data = "On";
    else
      data = "Off";
    return PROCESS_NORMAL; 
  }
  else
    return FRAME_ERR;
}


