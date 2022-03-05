
#include "imu.h"




#define Kp    120.0f
#define Ki    0.0f 
#define halfT 0.0025f	






float invSqrt(float x)	/*快速开平方求倒*/
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**
  * @brief  get eula angles from accelermeter
  * @param  axis6: accelermeter structure
  *         eula3: eula structure ptr
  * @retval none 
  * @date   2021-02-24
  */
void imu_get_eula(Axis6_t axis6,Eula3_t* eula3)
{
  eula3->roll = RADTODEG(FastAtan2(-axis6.ax,axis6.az));
	eula3->pitch = RADTODEG(FastAtan2(axis6.ay,axis6.az));
}

/**
  * @brief  get direction
  * @param  dir  : direction ptr
  *         eula3: eula structure ptr
  * @retval none 
  * @date   2022-02-07
  */
void imu_get_dir(Eula3_t* eula3, dir_e* dir)
{

	if(eula3->pitch<10.0f && eula3->pitch>-10.0f)
  {
		if(eula3->roll<-15.0f)     *dir = FRWD;
	  if(eula3->roll>15.0f)      *dir = BKWD;
	}
	
	if(eula3->roll<10.0f && eula3->roll>-10.0f)
  {
		if(eula3->pitch<-15.0f)     *dir = RIGHT;
	  if(eula3->pitch>15.0f)      *dir = LEFT;
	}
	
	if(eula3->pitch<10.0f && eula3->pitch>-10.0f && 
		 eula3->roll<10.0f && eula3->roll>-10.0f)
	*dir = MID;
}

/**
  * @brief  mahony IMU update
  * @param  axis6: accelermeter structure
  *         eula3: eula structure ptr
  * @retval none 
  * @date   2021-02-22
  */
void imu_update(Axis6_t axis6, Eula3_t* eula3)
{
  static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;  // 四元数的元素，代表估计方向
  static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;    // 按比例缩小积分误差

  float norm;
  float vx, vy, vz;
  float ex, ey, ez; 
  
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q1q1 = q1*q1;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;

  float ax = axis6.ax;
  float ay = axis6.ay;
  float az = axis6.az;
  float gx = DEGTORAD(axis6.gx);
  float gy = DEGTORAD(axis6.gy);
  float gz = DEGTORAD(axis6.gz);


 
	norm = invSqrt(ax*ax + ay*ay + az*az);
 
  ax = ax * norm;                 
  ay = ay * norm;
  az = az * norm;      
 
  vx = 2.0f*(q1q3 - q0q2);
  vy = 2.0f*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3; 
 
 
  ex = (ay*vz - az*vy);
  ey = (az*vx - ax*vz);
  ez = (ax*vy - ay*vx);
 
 
 
 
  exInt = exInt + ex*Ki;
  eyInt = eyInt + ey*Ki;
  ezInt = ezInt + ez*Ki;
 
 
 
 
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;
 
 
 
 
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	 q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
 
 
 
  // 正常化四元
 
  norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
 
  q0 = q0 * norm;
  q1 = q1 * norm;
  q2 = q2 * norm;
  q3 = q3 * norm;
 
  
 
  eula3->roll = RADTODEG(-FastAsin(2.0f * q1 * q3 - 2.0f * q0* q2)); 
 
  eula3->pitch = RADTODEG(FastAtan2(2.0f * q2 * q3 + 2.0f * q0 * q1, q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3)); 
 
  //yaw = atan2(2.0f*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * RAD2DEG; 

}
