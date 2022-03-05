#ifndef _IMU_H
#define _IMU_H

#include "main.h"

void imu_update(Axis6_t axis6, Eula3_t* eula3);
void imu_get_eula(Axis6_t axis6,Eula3_t* eula3);
void imu_get_dir(Eula3_t* eula3, dir_e* dir);







#endif
