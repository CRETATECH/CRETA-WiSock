/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "state.h"
#include "button.h"
#include "device.h"

/***************************************************************************************
* LOCAL TYPEDEFS
***************************************************************************************/
typedef enum {
    STATE_CONFIG  = 0,
    STATE_CONTROL = ~STATE_CONFIG,
}fsm_t;

/***************************************************************************************
* LOCAL DEFINES
***************************************************************************************/
#define EEPROM_ADDRESS                0x10

/***************************************************************************************
* LOCAL FUNCTIONS PROTOTYPES
***************************************************************************************/
void EEPROM_Write_ConfigFlag(uint8_t pData);
void Wifi_Connect(void);

/***************************************************************************************
* LOCAL VARIABLES
***************************************************************************************/
fsm_t gState = STATE_CONTROL;
led_status_t LED_FLAG = LED_STATUS_OFF;

/***************************************************************************************
* PUBLIC FUNCTIONS
***************************************************************************************/
/**
 * Put it in setup func
 */
void stateSetup(void){
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
}
/**
 * Update gState
 */
void stateUpdate(void){
    /* gState update (if, elseif,...) */
}
/**
 * Check gState and run
 */
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
/**
 * Config wifi via SmartConfig
 */
void stateConfig(void){
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

void stateControl(void){
    /* Check router connect */

}

/***************************************************************************************
* LOCAL FUNCTIONS
***************************************************************************************/
void EEPROM_Write_Config_Flag(uint8_t pData){
    EEPROM.begin(50);
    EEPROM.write(EEPROM_ADDRESS, pData);
    EEPROM.commit();
}

uint8_t EEPROM_Read_Config_Flag(void){
    EEPROM.begin(50);
    return EEPROM.read(EEPROM_ADDRESS);
}

void Wifi_Connect (void){
    uint8_t _retry = 10;
    WiFi.begin();
    while ((WiFi.status() != WL_CONNECTED)&(_retry != 0)){
        _retry--;
        delay(100);
    }
}