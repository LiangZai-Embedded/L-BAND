#ifndef _IMU_H
#define _IMU_H

#include "main.h"

void imu_update(Axis6_t axis6, Euler3_t* euler3);
void imu_get_euler(Axis6_t axis6,Euler3_t* euler3);
void imu_get_dir(Euler3_t* euler3, dir_e* dir);
void imu_get_yaw(Euler3_t* euler3,float* MagBuffer);







#endif
