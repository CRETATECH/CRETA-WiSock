#ifndef WISHOCK_H
#define WISHOCK_H

/*************************************************/
/*                  DEFINE SYMBOL                */
/*************************************************/
#define PIN_BUTTON_CONFIG             13
#define PIN_BUTTON_CONTROL            5
#define PIN_LED                       4
#define PIN_DEVICE                    3
#define EEPROM_ADDRESS                0x10
#define DEBUG            

typedef enum {
  STATE_CONFIG  = 0,
  STATE_CONTROL = ~STATE_CONFIG,
}fsm_t;

typedef enum {
  LED_STATUS_BLINK = 0,
  LED_STATUS_ON ,
  LED_STATUS_OFF,
}led_status_t;

typedef struct {
  String ID;
  String func;
  String addr;
  String data; 
}json_data_t;
/*************************************************/
/*                  GLOBAL VARIABLE              */
/*************************************************/
fsm_t g_state = STATE_CONTROL;
json_data_t g_data;
os_timer_t Timer;
led_status_t LED_FLAG = LED_STATUS_BLINK;

const char* mqtt_server = "iot.eclipse.org";
char topicIn[25];
char topicOut[25] ;

WiFiClient espClient;
PubSubClient client(espClient);
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
void func_config(void);

/*
 * connect to wifi, receive data, process and respond
 */
void func_control(void);

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
void EEPROM_Write_ConfigFlag (unsigned char value);

/*
 * Read EEPROM with ADDRESS = 0x10
 */
byte EEPROM_Read_ConfigFlag (void);

/*
 * Setup topic for MQTT
 */
void Topic_Set (void);

void Led_On (void);
void Led_Off (void);
void reconnect(void);
#endif
