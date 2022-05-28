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

}Coeff_t;



typedef struct
{
  Coeff_t coeff;

  float   x[3];
  float   y[3];

}BiquadFliter_t;






#endif
