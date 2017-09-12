#ifndef WISHOCK_H
#define WISHOCK_H

/*************************************************/
/*                  DEFINE SYMBOL                */
/*************************************************/
#define PIN_BUTTON_CONFIG             13
#define PIN_BUTTON_CONTROL            5
#define PIN_LED                       4
#define PIN_DEVICE                    3
//#define EEPROM_ADDRESS                0x10
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

//typedef struct {
//  String ID;
//  String func;
//  String addr;
//  String data; 
//}json_data_t;


#endif
