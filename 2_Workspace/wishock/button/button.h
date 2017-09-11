#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "../wishock.h"

void buttonInit(void);
void buttonConfigISRHandler(void);
bool buttonConfigCheck(void);

#endif