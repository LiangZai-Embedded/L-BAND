#ifndef _DataYPE_H
#define _DataYPE_H

#include "main.h"

typedef enum
{
	MID = 0,
	FRWD,
	BKWD,
	LEFT,
	RIGHT
}dir_e;	


typedef struct 
{
	float  temperature;
	float  batt;
	
	uint8_t year;
  uint8_t month;
	uint8_t week;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

  
	//ahrs
	float   roll;
  float   pitch;
  float   az;     
	float   yaw;
  dir_e   dir;
  
	uint16_t step;
	
	
 
}Data;
#endif
