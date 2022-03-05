#include "filter.h"

BiquadFliter_t lpf_acc,lpf_gyro;

/**
  * @brief  biquad filter initialization 
  * @param  none
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter_init(void)
{
    //memset(&lpf_acc,0,sizeof(BiquadFliter_t));
    //memset(&lpf_gyro,0,sizeof(BiquadFliter_t));

 

    //10HZ
    lpf_acc.gain = 0.020083365564211236f;  
    lpf_acc.B[0] = 1;
    lpf_acc.B[1] = 2;
    lpf_acc.B[2] = 1;
    lpf_acc.A[0] = 1;
    lpf_acc.A[1] = -1.5610180758007182f;
    lpf_acc.A[2] =   0.64135153805756318f;

    //10HZ
    lpf_gyro.gain = 0.020083365564211236f;  
    lpf_gyro.B[0] = 1;
    lpf_gyro.B[1] = 2;
    lpf_gyro.B[2] = 1;
    lpf_gyro.A[0] = 1;
    lpf_gyro.A[1] = -1.5610180758007182f;
    lpf_gyro.A[2] =   0.64135153805756318f;
}



/**
  * @brief  biquad filter
  * @param  filter: filter structure ptr
  *         axis6_out: 6axis output structure  ptr
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter(BiquadFliter_t* filter,float* x1,float* x2,float* x3)
{
 float A[3] = {0.0f}; 
 float B[3] = {0.0f};
 float w0x[3] = {0.0f}; 
 float w0y[3] = {0.0f};
 float w0z[3] = {0.0f};
 float w1x[3] = {0.0f};
 float w1y[3] = {0.0f};
 float w1z[3] = {0.0f};

 float gain = filter->gain;

 for(uint8_t i=0;i<3;i++)
 {
     A[i] = filter->A[i];
     B[i] = filter->B[i];
   w0x[i] = filter->w0x[i];
   w0y[i] = filter->w0y[i];
   w0z[i] = filter->w0z[i];
   w1x[i] = filter->w0x[i];
   w1y[i] = filter->w0y[i];
   w1z[i] = filter->w0z[i];
 }
 
 w0x[0] = *x1;
 w1x[0] = (B[0]*w0x[0]+B[1]*w0x[1]+B[2]*w0x[2])*gain-w1x[1]*A[1]-w1x[2]*A[2];
 *x1 = w1x[0]/A[0];

 w0y[0] = *x2;
 w1y[0] = (B[0]*w0y[0]+B[1]*w0y[1]+B[2]*w0y[2])*gain-w1y[1]*A[1]-w1y[2]*A[2];
 *x2 = w1y[0]/A[0];

 w0z[0] = *x3;
 w1z[0] = (B[0]*w0z[0]+B[1]*w0z[1]+B[2]*w0z[2])*gain-w1z[1]*A[1]-w1z[2]*A[2];
 *x3 = w1z[0]/A[0];

 w0x[2]=w0x[1];
 w0x[1]=w0x[0];
 w1x[2]=w1x[1];
 w1x[1]=w1x[0];

 w0y[2]=w0y[1];
 w0y[1]=w0y[0];
 w1y[2]=w1y[1];
 w1y[1]=w1y[0];

 w0z[2]=w0z[1];
 w0z[1]=w0z[0];
 w1z[2]=w1z[1];
 w1z[1]=w1z[0];


 for(uint8_t i=0;i<3;i++)
 {
   filter->w0x[i] = w0x[i];
   filter->w0y[i] = w0y[i];
   filter->w0z[i] = w0z[i];
   filter->w0x[i] = w1x[i];
   filter->w0y[i] = w1y[i];
   filter->w0z[i] = w1z[i];
 }


}



/**
  * @brief  biquad filter application
  * @param  axis6_in: accelermeter structure input
  *         axis6_out: accelermeter structure output
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter_apply(Axis6_t* axis6_in, Axis6_t* axis6_out)
{
  float ax = axis6_in->ax;
  float ay = axis6_in->ay;
  float az = axis6_in->az;
  float gx = axis6_in->gx;
  float gy = axis6_in->gy;
  float gz = axis6_in->gz;



  biquad_filter(&lpf_acc,&ax,&ay,&az);
  biquad_filter(&lpf_gyro,&gx,&gy,&gz);

  axis6_out->ax = ax; 
  axis6_out->ay = ay; 
  axis6_out->az = az; 
  axis6_out->gx = gx; 
  axis6_out->gy = gy; 
  axis6_out->gz = gz; 
}

