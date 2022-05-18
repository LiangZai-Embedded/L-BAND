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

   float x1[3];
   float x2[3];
   float x3[3];

   float y1[3];
   float y2[3];
   float y3[3];
}BiquadFliter_t;



extern BiquadFliter_t lpf_acc,lpf_gyro;



#endif
