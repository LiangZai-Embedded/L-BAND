#ifndef _HMC5883L_H
#define _HMC5883L_H		

#include "main.h"

#define	HMC5883L_ADDR                  0x3C	

#define HMC5883l_CONFIG_A              0x00
#define HMC5883l_CONFIG_B              0x01
#define HMC5883l_MODECONFIG            0x02

void  hmc5883l_init(void);
void hmc5883l_get_data(Euler3_t* euler3); 
float hmc5883l_read(void);
void hmc5883l_read_mxyz(float* mag);

#endif





