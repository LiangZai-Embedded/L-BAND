
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
  *         euler3: eula structure ptr
  * @retval none 
  * @date   2021-02-24
  */
void imu_get_euler(Axis6_t axis6,Euler3_t* euler3)
{
  euler3->roll = RADTODEG(FastAtan2(-axis6.ax,axis6.az));
	euler3->pitch = RADTODEG(FastAtan2(axis6.ay,axis6.az));
}

/**
  * @brief  get direction
  * @param  dir  : direction ptr
  *         euler3: eula structure ptr
  * @retval none 
  * @date   2022-02-07
  */
#define LIMIT     10.0f
#define THRESHOLD 16.0f
void imu_get_dir(Euler3_t* euler3, dir_e* dir)
{

	if(euler3->pitch<LIMIT && euler3->pitch>-LIMIT)
  {
		if(euler3->roll<-THRESHOLD)     *dir = FRWD;
	  if(euler3->roll>THRESHOLD)      *dir = BKWD;
	}
	
	if(euler3->roll<LIMIT && euler3->roll>-LIMIT)
  {
		if(euler3->pitch<-THRESHOLD)     *dir = RIGHT;
	  if(euler3->pitch>THRESHOLD)      *dir = LEFT;
	}
	
	if(euler3->pitch<LIMIT && euler3->pitch>-LIMIT && 
		 euler3->roll<LIMIT && euler3->roll>-LIMIT)
	*dir = MID;

}

//float Data_conversion(float *AccBuffer,float *MagBuffer)
//{
//  unsigned char i;
//
//  volatile float fNormAcc,fSinRoll,fCosRoll,fSinPitch,fCosPitch = 0.0f;
//  float fTiltedX,fTiltedY = 0.0f;
//  float fcosf=0;
//  float HeadingValue = 0.0f;
//
//  for(i=0;i<3;i++)
//	  AccBuffer[i] /= 100.0f;
//
//
//
//
//      fNormAcc = FastSqrt((AccBuffer[0]*AccBuffer[0])+(AccBuffer[1]*AccBuffer[1])+(AccBuffer[2]*AccBuffer[2]));
//
//      fSinRoll = AccBuffer[1]/fNormAcc;
//
//
//
//      fCosRoll = FastSqrt(1.0-(fSinRoll * fSinRoll));
//      fSinPitch = AccBuffer[0]/fNormAcc;
//      fCosPitch = FastSqrt(1.0-(fSinPitch * fSinPitch));
//
//
//      fTiltedX = MagBuffer[0]*fCosPitch + MagBuffer[2]*fSinPitch;
//      fTiltedY = MagBuffer[0]*fSinRoll*fSinPitch + MagBuffer[1]*fCosRoll - MagBuffer[2]*fSinRoll*fCosPitch;
//
//      fcosf=fTiltedX /FastSqrt(fTiltedX*fTiltedX+fTiltedY*fTiltedY);
//
////			if(fTiltedY>0)
////			  HeadingValue = (float)(acos(fcosf)*180/PI);
////			else
////				HeadingValue =360-(float)(acos(fcosf)*180/PI);
//
//      HeadingValue =FastAtan2((float)fTiltedY,(float)fTiltedX)*180/PI;
//      HeadingValue+=11;//地磁的北极和地球的北极相差11度左右。
//			if(HeadingValue>360)
//				HeadingValue=HeadingValue-360;
//
//	    return HeadingValue ;
//}


void imu_get_yaw(Euler3_t* euler3,float* MagBuffer)
{
	float mag_offset[3]={0.0f};
	static float mag_max[3] = {-2000.0f,-2000.0f,-2000.0f};
	static float mag_min[3] = {2000.0f,2000.0f,2000.0f};
	float fSinRoll,fCosRoll,fSinPitch,fCosPitch;
	float fTiltedX,fTiltedY = 0.0f;
	float HeadingValue = 0.0f;

 	for(uint8_t i=0;i<3;i++)
    {
 		if(MagBuffer[i]>mag_max[i])
 			mag_max[i] = MagBuffer[i];

	    if(MagBuffer[i]<mag_min[i])
	    	mag_min[i] = MagBuffer[i];
    }

 	mag_offset[0] = (mag_max[0]+mag_min[0])/2.0f;
 	mag_offset[1] = (mag_max[1]+mag_min[1])/2.0f;
 	mag_offset[2] = (mag_max[2]+mag_min[2])/2.0f;

 	MagBuffer[0] -= mag_offset[0];
 	MagBuffer[1] -= mag_offset[1];
 	MagBuffer[2] -= mag_offset[2];


 	fSinRoll = FastSin(DEGTORAD(euler3->pitch));
 	fSinPitch = FastSin(DEGTORAD(euler3->roll));

 	fCosRoll = FastCos(DEGTORAD(euler3->pitch));
 	fCosPitch = FastCos(DEGTORAD(euler3->roll));

 	fTiltedX = MagBuffer[0]*fCosPitch + MagBuffer[2]*fSinPitch;
 	fTiltedY = MagBuffer[0]*fSinRoll*fSinPitch + MagBuffer[1]*fCosRoll - MagBuffer[2]*fSinRoll*fCosPitch;


 	euler3->yaw =FastAtan2((float)fTiltedY,(float)fTiltedX)*180/PI;


}
