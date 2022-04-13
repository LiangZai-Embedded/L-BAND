#ifndef _FILTER_H
#define _FILTER_H

#include "main.h"

void biquad_filter_init(void);
void biquad_filter_apply(Axis6_t* axis6_in, Axis6_t* axis6_out);

typedef struct
{
   float B[3];
   float A[3];
   
   float gain;

   float w0x[3];
   float w0y[3];
   float w0z[3];

   float w1x[3];
   float w1y[3];
   float w1z[3];
}BiquadFliter_t;



extern BiquadFliter_t lpf_acc,lpf_gyro,notch1_acc,notch2_acc;



#endif
