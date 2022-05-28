
#ifndef _TASK_H
#define _TASK_H

#include "main.h"

#define  VIB  ((data.az > 9.5f) && data.dir==MID) //vibration threshold
#define  SWAY (data.az > 50.0f)                     //intense sway

#define  QUIT  (data.dir==FRWD||data.dir==BKWD)

extern Data data;
extern uint32_t tick;// increase 1 every 1 ms in TIM IT ISR
extern uint8_t alarm[3];
extern Axis6_t axis6_export;

extern uint32_t tick;


typedef struct
{
	FlagStatus runflag;
	uint32_t cnt;
	uint32_t reload;
	void (*excute) (void);

}task_t;



void task_scheduler(void);															





#endif





























