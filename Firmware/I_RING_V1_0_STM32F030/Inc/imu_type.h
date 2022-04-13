#ifndef _IMU_TYPE_H
#define _IMU_TYPE_H


#include "main.h"

typedef struct
{
  float pitch;
  float roll;
  float yaw;
}Euler3_t;


typedef struct
{
  float ax;
  float ay;
  float az;
  
  float gx;
  float gy;
  float gz;
}Axis6_t;


typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
}Axis3_t;
#endif
