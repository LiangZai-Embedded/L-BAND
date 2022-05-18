#ifndef _DATA_H
#define _DATA_H

#include "main.h"


#define  VIB  ((data.az > 11.0f) && data.dir==MID) //vibration threshold
#define  SWAY (data.az > 50.0f)                     //intense sway

#define  QUIT  (data.dir==FRWD||data.dir==BKWD)

extern Data data;
extern uint32_t tick;// increase 1 every 1 ms in TIM IT ISR
extern uint8_t alarm[3];

void data_get(Data* data);




#endif
