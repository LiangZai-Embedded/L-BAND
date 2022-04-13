//#include "hmc5883l.h"



//void  hmc5883l_init(void)
//{
//    unsigned char buf[3]={0x70,0x20,0X00};
//		
//		HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_CONFIG_A, I2C_MEMADD_SIZE_8BIT,buf, 1, 100);           
//		HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_CONFIG_B, I2C_MEMADD_SIZE_8BIT, buf+1, 1, 100);         
//    HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_MODECONFIG, I2C_MEMADD_SIZE_8BIT, buf+2, 1, 100);    
//}


/////**
////  * @brief  reg 3-DOF magdata from qmc5883
////  * @param  mag_buff[x,y,z]
////  * @retval none  
////	* @date   2020-09-13
////  */
////static void hmc5883l_read_mxyz(int16_t* mag)
////{
////	uint8_t buf[6];
////  
////	HAL_I2C_Mem_Read(&hi2c1,HMC5883L_ADDR, 0x03,1,buf,6 , 1000); 
////  
////	mag[0] = (((int16_t)buf[0]<<8)|buf[1]);
////	mag[1] = (((int16_t)buf[4]<<8)|buf[5]);
////  mag[2] = (((int16_t)buf[2]<<8)|buf[3]);
////}




//float hmc5883l_read(void)
//{
//  uint8_t dat[6];
//	
//  int16_t mag_raw[3] = {0};
//	float   mag_out[3] = {0.0f};
//	float   mag_offset[3]={0.0f};
//  
//  static int16_t mag_max[3] = {-2048,-2048,-2048};
//	static int16_t mag_min[3] = {2047,2047,2047};
//	


//	HAL_I2C_Mem_Read(&hi2c1,HMC5883L_ADDR,0x03,1,dat,6,1000); 
//  
//	mag_raw[0] = (dat[0] << 8) | dat[1]; //mx 
//  mag_raw[1] = (dat[4] << 8) | dat[5]; //my
//  mag_raw[2] = (dat[2] << 8) | dat[3]; //mz

// 	for(uint8_t i=0;i<3;i++)
//	{
//		if(mag_raw[i]>mag_max[i])  mag_max[i] = mag_raw[i];
//		if(mag_raw[i]<mag_min[i])  mag_min[i] = mag_raw[i];
//	}
//	
//  mag_offset[0] = (mag_max[0]+mag_min[0])/2;
//  mag_offset[1] = (mag_max[1]+mag_min[1])/2;
//	mag_offset[2] = (mag_max[2]+mag_min[2])/2;
//  
//	mag_raw[0] -= mag_offset[0];
//  mag_raw[1] -= mag_offset[1];
//  mag_raw[2] -= mag_offset[2];
//	
//	mag_out[0] = (float)mag_raw[0]/1090.0f;
//	mag_out[1] = (float)mag_raw[1]/1090.0f;
//	mag_out[2] = (float)mag_raw[2]/1090.0f;

//	return RADTODEG(FastAtan2(mag_out[1],mag_out[0]));

//}



////void hmc5883l_get_data(Euler3_t* euler3) 
////{    
////   int16_t mag[3] = {0};
////   
////   float mx,my,mz,hy,hx;
////   
////   float roll = (euler3->pitch);
////   float pitch = -(euler3->roll);
////   
////   float cos_pitch,sin_pitch,cos_roll,sin_roll;
////   
////   FastSinCos(pitch,&sin_pitch,&cos_pitch);
////   FastSinCos(roll,&sin_roll,&cos_roll);


////	 hmc5883l_read_mxyz(mag);
////	 
////   roll = DEGTORAD(roll);
////   pitch = DEGTORAD(pitch);

////     
//////   mag[0] *= magGain[0];
//////   mag[1] *= magGain[1];
//////   mag[2] *= magGain[2];
////     
//////   mag[0] -= mag_offset[0];
//////   mag[1] -= mag_offset[1];
//////   mag[2] -= mag_offset[2];

////	 mx = (float)mag[0];//1090.0f;
////   my = (float)mag[1];//1090.0f;
////   mz = (float)mag[2];//1090.0f;
////   
////	 hx = mx * cos_pitch + my * sin_roll * sin_pitch + mz * cos_roll * sin_pitch;
////   hy = my * cos_roll - mz * sin_roll;
////   
////   euler3->yaw = RADTODEG(FastAtan2(hy, hx)); 
////}



#include "hmc5883l.h"


void  hmc5883l_init(void)
{
    unsigned char buf[3]={0x78,0x20,0x00};
		
		HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_CONFIG_A, I2C_MEMADD_SIZE_8BIT,buf, 1, 100);           
		HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_CONFIG_B, I2C_MEMADD_SIZE_8BIT, buf+1, 1, 100);         
    HAL_I2C_Mem_Write(&hi2c1,HMC5883L_ADDR,HMC5883l_MODECONFIG, I2C_MEMADD_SIZE_8BIT, buf+2, 1, 100);    
}


/**
  * @brief  read 3-DOF magdata from qmc5883
  * @param  mag data buffer
  * @retval none  
	* @date   2020-09-13
  */
void hmc5883l_read_mxyz(float* mag)
{
	uint8_t buf[6];
  
  HAL_I2C_Mem_Read(&hi2c1,HMC5883L_ADDR, 0x03,1,buf,6 , 1000);
	
	mag[0] = (float)((int16_t)(((uint16_t)buf[0]<<8 | buf[1])))/1090.0f;
	mag[1] = (float)((int16_t)(((uint16_t)buf[4]<<8 | buf[5])))/1090.0f;
	mag[2] = (float)((int16_t)(((uint16_t)buf[2]<<8 | buf[3])))/1090.0f;
}




float hmc5883l_read(void)
{	
  float mag_raw[3] = {0.0f};
	float mag_offset[3]={0.0f};
  
  static float mag_max[3] = {-2.0f,-2.0f,-2.0f};
	static float mag_min[3] = {2.0f,2.0f,2.0f};
	

  hmc5883l_read_mxyz(mag_raw);


 	for(uint8_t i=0;i<3;i++)
	{
		if(mag_raw[i]>mag_max[i])  
			mag_max[i] = mag_raw[i];
		
		if(mag_raw[i]<mag_min[i])  
			mag_min[i] = mag_raw[i];
	}
	
  mag_offset[0] = (mag_max[0]+mag_min[0])/2.0f;
  mag_offset[1] = (mag_max[1]+mag_min[1])/2.0f;
	mag_offset[2] = (mag_max[2]+mag_min[2])/2.0f;
  
	mag_raw[0] -= mag_offset[0];
  mag_raw[1] -= mag_offset[1];
  mag_raw[2] -= mag_offset[2];
	

	return RADTODEG(FastAtan2(mag_raw[1],mag_raw[0]));

}



void hmc5883l_get_data(Euler3_t* euler3) 
{    
  float mag[3] = {0.0f};
	float mag_offset[3]={0.0f};
  
  static float mag_max[3] = {-2.0f,-2.0f,-2.0f};
	static float mag_min[3] = {2.0f,2.0f,2.0f};
   
   float mx,my,mz,hy,hx;
   
   float roll = -(euler3->roll);
   float pitch = -(euler3->pitch);
   
   float cos_pitch,sin_pitch,cos_roll,sin_roll;
   



	 hmc5883l_read_mxyz(mag);
	 	for(uint8_t i=0;i<3;i++)
	{
		if(mag[i]>mag_max[i])  
			mag_max[i] = mag[i];
		
		if(mag[i]<mag_min[i])  
			mag_min[i] = mag[i];
	}
	
  mag_offset[0] = (mag_max[0]+mag_min[0])/2.0f;
  mag_offset[1] = (mag_max[1]+mag_min[1])/2.0f;
	mag_offset[2] = (mag_max[2]+mag_min[2])/2.0f;
  
	mag[0] -= mag_offset[0];
  mag[1] -= mag_offset[1];
  mag[2] -= mag_offset[2];
	 
   roll = DEGTORAD(roll);
   pitch = DEGTORAD(pitch);
   
	 FastSinCos(pitch,&sin_pitch,&cos_pitch);
   FastSinCos(roll,&sin_roll,&cos_roll);



	 mx = mag[0];
   my = mag[1];
   mz = mag[2];
   
	 hx = mx * cos_pitch + my * sin_roll * sin_pitch + mz * cos_roll * sin_pitch;
   hy = my * cos_roll - mz * sin_roll;
   
   euler3->yaw = RADTODEG(FastAtan2(-hy,hx)); 
}
