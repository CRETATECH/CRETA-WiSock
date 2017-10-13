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
/**
 * @brief       Initialize protocol
 * @param       none
 * @retval      None
 *              
 */
void protocolInit(void)
{


}

/**
 * @brief       parse Json
 * @param       pJson
 * @retval      0: parse failed
 *              1: parse success
 */
int jsonParse(String pJson)
{
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& root = _jsonBuffer.parseObject(pJson);
  if (!root.success())
  {
    #ifdef DEBUG
      Serial.println("pars fail");
    #endif
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
  #ifdef DEBUG
    Serial.println("pars ok");
  #endif
  return 1; 
}

/**
 * @brief       create Json to prepare for publishing
 * @param       pFunc, pAddr, pData
 * @retval      String json
 *              
 */
String protocolCreateJson (String pFunc, String pAddr, String pData)
{
  
   String _stringout = "{\"ID\" : \"ESP"  + Get_macID() + "\", \"FUNC\" : \"" + pFunc + "\", \"ADDR\" : \"" + pAddr + "\", \"DATA\" : \"" + pData + "\"}";
   return _stringout;
}

/**
 * @brief       process data when button is pressed
 * @param       none
 * @retval      None
 *              
 */
void protocolButtonProcess (void)
{
  uint32_t _time_out;
  _time_out = millis();
  if (deviceStatus() == DEVICE_ON)
  {
    deviceOff();
    ledDeviceOff();
    while (!mqttConnected())
    {
      if ((millis() - _time_out) > 300)
        break;
    }
      mqttPublish(protocolCreateJson("Data", "1", "Off")); 
  }
  else
  {
    deviceOn();
    ledDeviceOn();
    while (!mqttConnected())
    {
      if ((millis() - _time_out) > 300)
        break;
    }
      mqttPublish(protocolCreateJson("Data", "1", "On")); 
  }
}

/**
 * @brief       process data received from server
 * @param       pJsonRecv
 * @retval      None
 *              
 */
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
      #ifdef DEBUG
        Serial.println("func ctrl");
      #endif
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
      #ifdef DEBUG
        Serial.println("func data");
      #endif
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

/**
 * @brief       process Ctrl function
 * @param       none
 * @retval      PROCESS_NORMAL
 *              PROCESS_ERR
 *              FRAME_ERR
 */
int protocolCtrlFunc_Process (void)
{
  if (gAddr == "1")
  {
    if (gData == "On")
    {
      deviceOn();
      delay(50);
      if (deviceStatus() == DEVICE_ON)
      //if (1 == 1)
      {
        ledDeviceOn();
        return PROCESS_NORMAL;
      }
      else return PROCESS_ERR;
    }
    else if (gData == "Off")
    {
      deviceOff();
      delay(50);
      if (deviceStatus() == DEVICE_OFF)
      //if (1 == 1)
      {
        ledDeviceOff();
        return PROCESS_NORMAL;
      }
      else return PROCESS_ERR;
    }
    else return FRAME_ERR;
  }
  else return FRAME_ERR;
}

/**
 * @brief       process Data function
 * @param       none
 * @retval      PROCESS_NORMAL
 *              FRAME_ERR
 */
int protocolDataFunc_Process (void)
{
  if (gAddr == "1")
  {
    if (deviceStatus() == DEVICE_ON)
    //if (1 == 1)
      gData = "On";
    else
      gData = "Off";
    return PROCESS_NORMAL; 
  }
  else
    return FRAME_ERR;
}


