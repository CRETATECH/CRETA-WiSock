#ifndef __DEVICE_H__
#define __DEVICE_H__

/***************************************************************************************
* INCLUDE
***************************************************************************************/
#include "wishock.h"

/***************************************************************************************
* PUBLIC FUNCTION PROTOTYPES
***************************************************************************************/
void deviceInit(void);
void deviceOn(void);
void deviceOff(void);
void deviceToggle(void);
void ledOn(void);
void ledOff(void);
void ledToggle(void);

#endif
