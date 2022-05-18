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
    memset(&lpf_acc,0,sizeof(BiquadFliter_t));
    memset(&lpf_gyro,0,sizeof(BiquadFliter_t));

 

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
  *         in1,in2,in3:3-axis input ptr
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter(BiquadFliter_t* filter,float* in1,float* in2,float* in3)
{
 float A[3] = {0.0f}; 
 float B[3] = {0.0f};
 float x1[3] = {0.0f}; 
 float x2[3] = {0.0f};
 float x3[3] = {0.0f};
 float y1[3] = {0.0f};
 float y2[3] = {0.0f};
 float y3[3] = {0.0f};

 float gain = filter->gain;

 for(uint8_t i=0;i<3;i++)
 {
     A[i] = filter->A[i];
     B[i] = filter->B[i];
   
	 x1[i] = filter->x1[i];
   y1[i] = filter->y1[i];
	 
	 x2[i] = filter->x2[i];
   y2[i] = filter->y2[i];

	 
	 x3[i] = filter->x3[i];
   y3[i] = filter->y3[i];
 }
 
 x1[2]=x1[1];
 x1[1]=x1[0];
 y1[2]=y1[1];
 y1[1]=y1[0];

 x2[2]=x2[1];
 x2[1]=x2[0];
 y2[2]=y2[1];
 y2[1]=y2[0];

 x3[2]=x3[1];
 x3[1]=x3[0];
 y3[2]=y3[1];
 y3[1]=y3[0];
 
 
 
 x1[0] = *in1;
 y1[0] = (gain*(B[0]*x1[0]+B[1]*x1[1]+B[2]*x1[2]) - A[1]*y1[1] - A[2]*y1[2])/A[0];
 *in1 = y1[0];

 x2[0] = *in2;
 y2[0] = (gain*(B[0]*x2[0]+B[1]*x2[1]+B[2]*x2[2]) - A[1]*y2[1] - A[2]*y2[2])/A[0];
 *in2 = y2[0];


 x3[0] = *in3;
 y3[0] = (gain*(B[0]*x3[0]+B[1]*x3[1]+B[2]*x3[2]) - A[1]*y3[1] - A[2]*y3[2])/A[0];
 *in3 = y3[0];





 for(uint8_t i=0;i<3;i++)
 {
   filter->x1[i] = x1[i];
   filter->x2[i] = x2[i];
   filter->x3[i] = x3[i];
   filter->y1[i] = y1[i];
   filter->y2[i] = y2[i];
   filter->y3[i] = y3[i];
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

