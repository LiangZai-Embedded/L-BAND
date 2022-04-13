#include "icm20602.h"





void icm20602_self1_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //读取ICM20602 ID
    {
        HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R, ICM20602_WHO_AM_I,I2C_MEMADD_SIZE_8BIT,&dat,1,10); 

        HAL_Delay(10);
    }
}



void icm20602_init(void)
{
	  uint8_t data;
    HAL_Delay(10);  //上电延时
    
    //检测
    icm20602_self1_check();
    
    //复位
	  data = 0x80;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);           
    
    data = 0;                                                     //延时
    while(data!=0x41)
			 HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R,ICM20602_PWR_MGMT_1,I2C_MEMADD_SIZE_8BIT,&data,1,10);//等待复位完成
    
    //配置参数
		
		data = 0x01;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);           
    
	  data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);           

		data = 0x02;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_CONFIG, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);           

		data = 0x01;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);         
    
		data = 0x18;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);  
    
		data = 0x10;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);  
		
		data = 0x02;
		HAL_I2C_Mem_Write(&hi2c1,ICM20602_ADDR_W,ICM20602_ACCEL_CONFIG_2, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);  
}





void icm20602_get_acc_int16(Axis3_t* axis3)
{
	uint8_t dat[6];
	
	HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R,ICM20602_ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,dat,6,10);

  
	axis3->x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
  axis3->y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
  axis3->z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]);
}


static void icm20602_get_acc(Axis6_t* axis6)
{
	uint8_t dat[6];
	
	HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R,ICM20602_ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,dat,6,10);

  
	axis6->ax = (float)((int16_t)(((uint16_t)dat[0]<<8 | dat[1])))*0.002439f;
  axis6->ay = (float)((int16_t)(((uint16_t)dat[2]<<8 | dat[3])))*0.002439f;
  axis6->az = (float)((int16_t)(((uint16_t)dat[4]<<8 | dat[5])))*0.002439f;
}


static void icm20602_get_gyro(Axis6_t* axis6)
{
 uint8_t dat[6];

 HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R,ICM20602_GYRO_XOUT_H,I2C_MEMADD_SIZE_8BIT,dat,6,10);
  
 axis6->gx = (float)((int16_t)(((uint16_t)dat[0]<<8 | dat[1])))*0.061035f;// 2000dps/32767
 axis6->gy = (float)((int16_t)(((uint16_t)dat[2]<<8 | dat[3])))*0.061035f;
 axis6->gz = (float)((int16_t)(((uint16_t)dat[4]<<8 | dat[5])))*0.061035f;
}



void icm20602_get_axis6_data(Axis6_t* axis6)
{	
  float gx_bias = -0.639160f,gy_bias = -0.645508f,gz_bias = -0.014526f;
	
	icm20602_get_acc(axis6);
	icm20602_get_gyro(axis6);

	axis6->gx = axis6->gx - gx_bias;
  axis6->gy = axis6->gy - gy_bias;
	axis6->gz = axis6->gz - gz_bias;
}


void icm20602_get_temperature(float* temperature)
{
	uint8_t buf[2] = {0};
	 
	HAL_I2C_Mem_Read(&hi2c1,ICM20602_ADDR_R,ICM20602_TEMP_OUT_H,I2C_MEMADD_SIZE_8BIT,buf,2,10);

	*temperature = (float)((int16_t)(buf[0]<<8 | buf[1]))/326.8f + 25.0f;
}
