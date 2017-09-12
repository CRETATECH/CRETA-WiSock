/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include "user_interface.h"
#include "state.h"
#include "wishock.h"
#include "button.h"
#include "device.h"
#include "protocol.h"
#include "mqtt.h"
/***************************************************************************************
* LOCAL TYPEDEFS
***************************************************************************************/


/***************************************************************************************
* LOCAL DEFINES
***************************************************************************************/
#define EEPROM_ADDRESS                0x10

/***************************************************************************************
* LOCAL FUNCTIONS PROTOTYPES
***************************************************************************************/
void EEPROM_Write_ConfigFlag(uint8_t pData);
void Wifi_Connect(void);
uint8_t EEPROM_Read_ConfigFlag(void);
void Timer_ISR (void);
void stateConfig(void);
void stateControl(void);
/***************************************************************************************
* LOCAL VARIABLES
***************************************************************************************/
fsm_t gState = STATE_CONTROL;
os_timer_t Timer;
led_status_t LED_FLAG;
/***************************************************************************************
* EXTERN VARIABLES
***************************************************************************************/


/***************************************************************************************
* PUBLIC FUNCTIONS
***************************************************************************************/
void stateMachine(void){
    switch(gState){
        case STATE_CONFIG:
            stateConfig();
        break;
        case STATE_CONTROL:
            stateControl();
        break;
        default:
        break;
    }
}

void stateUpdate(void){
    /* gState update (if, elseif,...) */
}


void stateSetup (void)
{
    int g_flag_config = 0;
    /* Setup serial */
    Serial.begin(115200);
    /* Setup button */
    buttonInit();
    /* Setup device */
    deviceInit();
    /* Setup timer */
    os_timer_disarm(&Timer);
    os_timer_setfn(&Timer, (os_timer_func_t *)Timer_ISR, NULL);
    os_timer_arm(&Timer, 200, 1);
    /* Anything else here */


    protocolInit();
    mqttCreateTopic();
    if (EEPROM_Read_ConfigFlag() == 0x05)
      g_flag_config = 1; // configed
    else g_flag_config = 0; // not configed yet

    if (g_flag_config == 0)
      gState = STATE_CONFIG;
    else gState = STATE_CONTROL;
    gState = STATE_CONTROL;
}
/***************************************************************************************
* LOCAL FUNCTIONS
***************************************************************************************/
void stateConfig(void)
{
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
    gState = STATE_CONTROL;
}

void stateControl(void)
{
    /* Check router connect */
    if (WiFi.status() == WL_CONNECTED)
    {
      /* Check connect server */
      if (!mqttConnected())
      {
        if (mqttConnect())
        {
          mqttSubscribe();
          Serial.println("subscribe topic");
        }
      }
      if (mqttConnect())
      {
        mqttLoop();
      }
    }
    else Wifi_Connect();
}

void EEPROM_Write_ConfigFlag(uint8_t pData){
    EEPROM.begin(50);
    EEPROM.write(EEPROM_ADDRESS, pData);
    EEPROM.commit();
}

uint8_t EEPROM_Read_ConfigFlag(void){
    EEPROM.begin(50);
    return EEPROM.read(EEPROM_ADDRESS);
}

void Wifi_Connect (void)
{
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print('.');
  }
  Serial.println("da ket noi wifi");
}

void Timer_ISR (void)
{
  //Serial.println("Timer");
  if (LED_FLAG == LED_STATUS_BLINK)
  {
    ledToggle();
  }
  else if (LED_FLAG == LED_STATUS_ON)
    ledOn();
  else ledOff();
}
