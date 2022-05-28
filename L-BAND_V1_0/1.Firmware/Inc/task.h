
#ifndef _TASK_H
#define _TASK_H

#include "main.h"

extern uint32_t t;


typedef struct
{
	uint8_t runflag;
	uint32_t ct;
	uint32_t reload;
	void (*excute) (void);

}task_t;



void task_scheduler(void);															





#endif





























