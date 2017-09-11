#ifndef WISHOCK_H
#define WISHOCK_H

/*************************************************/
/*                  DEFINE SYMBOL                */
/*************************************************/
#define BUTTON_CONFIG             13
#define BUTTON_CONTROL            5
#define LED                       4
#define DEVICE                    3
#define EEPROM_ADDRESS            0x10
#define DEBUG                     0

enum finite_state_machine {
  STATE_CONFIG = 0,
  STATE_CONTROL,
};

typedef struct {
  String ID;
  String func;
  String addr;
  String data; 
}json_data_t;
/*************************************************/
/*                  GLOBAL VARIABLE              */
/*************************************************/
finite_state_machine g_state = STATE_CONTROL;
json_data_t g_data;
os_timer_t Timer;

const char* mqtt_server = "iot.eclipse.org";
String topicIn = "";
String topicOut = "";

/*************************************************/
/*                  FUCTION PROTOTYPE            */
/*************************************************/

/*
 *  Initialize Serial, Timer, GPIO
 */
void System_Init (void);

/*
 *  Interrupt Service Routine of BUTTON_CONFIG
 */
void ConfigButton_ISR(void);

/*
 *  Config Wifi, use smartconfig
 */
void func_state_config(void);

/*
 * connect to wifi, receive data, process and respond
 */
void func_state_control(void);

/*
 *  get macID of ESP8266
 */
String Get_macID (void);

/*
 *  Timer interrupt service routine
 */
void Timer_ISR (void);

/*
 * Connect to wifi
 */
void Wifi_Connect (void); 

/*
 * parse Json String
 */
int pars_json(String json);

/*
 *  Write byte toEEPROM with ADDRESS = 0x10
 */
void EEPROM_Write (unsigned char value);

/*
 * Read EEPROM with ADDRESS = 0x10
 */
byte EEPROM_Read (void);

/*
 * Setup topic for MQTT
 */
void Topic_Set (void);
 
#endif
