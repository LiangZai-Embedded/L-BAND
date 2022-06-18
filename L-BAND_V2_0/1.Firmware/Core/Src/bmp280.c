/**
 * Ciastkolog.pl (https://github.com/ciastkolog)
 * 
*/
/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 sheinz (https://github.com/sheinz)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "bmp280.h"

/**
 * BMP280 registers
 */
#define BMP280_REG_TEMP_XLSB   0xFC /* bits: 7-4 */
#define BMP280_REG_TEMP_LSB    0xFB
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_TEMP        (BMP280_REG_TEMP_MSB)
#define BMP280_REG_PRESS_XLSB  0xF9 /* bits: 7-4 */
#define BMP280_REG_PRESS_LSB   0xF8
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_PRESSURE    (BMP280_REG_PRESS_MSB)
#define BMP280_REG_CONFIG      0xF5 /* bits: 7-5 t_sb; 4-2 filter; 0 spi3w_en */
#define BMP280_REG_CTRL        0xF4 /* bits: 7-5 osrs_t; 4-2 osrs_p; 1-0 mode */
#define BMP280_REG_STATUS      0xF3 /* bits: 3 measuring; 0 im_update */
#define BMP280_REG_CTRL_HUM    0xF2 /* bits: 2-0 osrs_h; */
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_ID          0xD0
#define BMP280_REG_CALIB       0x88
#define BMP280_REG_HUM_CALIB   0x88

#define BMP280_RESET_VALUE     0xB6

BMP280_HandleTypedef bmp280;


void bmp280_init_default_params(bmp280_params_t *params) {
	params->mode = BMP280_MODE_NORMAL;
	params->filter = BMP280_FILTER_OFF;
	params->oversampling_pressure = BMP280_STANDARD;
	params->oversampling_temperature = BMP280_STANDARD;
	params->standby = BMP280_STANDBY_250;
}

static bool read_register16(BMP280_HandleTypedef *dev, uint8_t addr, uint16_t *value) {
	uint16_t tx_buff;
	uint8_t rx_buff[2];
	tx_buff = (dev->addr << 1);

	if (HAL_I2C_Mem_Read(dev->i2c, tx_buff, addr, 1, rx_buff, 2, 5000)
			== HAL_OK) {
		*value = (uint16_t) ((rx_buff[1] << 8) | rx_buff[0]);
		return true;
	} else
		return false;

}

static inline int read_data(BMP280_HandleTypedef *dev, uint8_t addr, uint8_t *value,
		uint8_t len) {
	uint16_t tx_buff;
	tx_buff = (dev->addr << 1);
	if (HAL_I2C_Mem_Read(dev->i2c, tx_buff, addr, 1, value, len, 5000) == HAL_OK)
		return 0;
	else
		return 1;

}

static bool read_calibration_data(BMP280_HandleTypedef *dev) {

	if (read_register16(dev, 0x88, &dev->dig_T1)
			&& read_register16(dev, 0x8a, (uint16_t *) &dev->dig_T2)
			&& read_register16(dev, 0x8c, (uint16_t *) &dev->dig_T3)
			&& read_register16(dev, 0x8e, &dev->dig_P1)
			&& read_register16(dev, 0x90, (uint16_t *) &dev->dig_P2)
			&& read_register16(dev, 0x92, (uint16_t *) &dev->dig_P3)
			&& read_register16(dev, 0x94, (uint16_t *) &dev->dig_P4)
			&& read_register16(dev, 0x96, (uint16_t *) &dev->dig_P5)
			&& read_register16(dev, 0x98, (uint16_t *) &dev->dig_P6)
			&& read_register16(dev, 0x9a, (uint16_t *) &dev->dig_P7)
			&& read_register16(dev, 0x9c, (uint16_t *) &dev->dig_P8)
			&& read_register16(dev, 0x9e,
					(uint16_t *) &dev->dig_P9)) {

		return true;
	}

	return false;
}



static int write_register8(BMP280_HandleTypedef *dev, uint8_t addr, uint8_t value) {
	uint16_t tx_buff;

	tx_buff = (dev->addr << 1);

	if (HAL_I2C_Mem_Write(dev->i2c, tx_buff, addr, 1, &value, 1, 10000) == HAL_OK)
		return false;
	else
		return true;
}

bool bmp280_init(BMP280_HandleTypedef *dev, bmp280_params_t *params) {

	if (dev->addr != BMP280_I2C_ADDRESS_0
			&& dev->addr != BMP280_I2C_ADDRESS_1) {

		return false;
	}

	if (read_data(dev, BMP280_REG_ID, &dev->id, 1)) {
		return false;
	}

	if (dev->id != BMP280_CHIP_ID && dev->id != BME280_CHIP_ID) {

		return false;
	}

	// Soft reset.
	if (write_register8(dev, BMP280_REG_RESET, BMP280_RESET_VALUE)) {
		return false;
	}

	// Wait until finished copying over the NVP data.
	while (1) {
		uint8_t status;
		if (!read_data(dev, BMP280_REG_STATUS, &status, 1)
				&& (status & 1) == 0)
			break;
	}

	if (!read_calibration_data(dev)) {
		return false;
	}



	uint8_t config = (params->standby << 5) | (params->filter << 2);
	if (write_register8(dev, BMP280_REG_CONFIG, config)) {
		return false;
	}

	if (params->mode == BMP280_MODE_FORCED) {
		params->mode = BMP280_MODE_SLEEP;  // initial mode for forced is sleep
	}

	uint8_t ctrl = (params->oversampling_temperature << 5)
			| (params->oversampling_pressure << 2) | (params->mode);



	if (write_register8(dev, BMP280_REG_CTRL, ctrl)) {
		return false;
	}

	return true;
}

bool bmp280_force_measurement(BMP280_HandleTypedef *dev) {
	uint8_t ctrl;
	if (read_data(dev, BMP280_REG_CTRL, &ctrl, 1))
		return false;
	ctrl &= ~0b11;  // clear two lower bits
	ctrl |= BMP280_MODE_FORCED;
	if (write_register8(dev, BMP280_REG_CTRL, ctrl)) {
		return false;
	}
	return true;
}

bool bmp280_is_measuring(BMP280_HandleTypedef *dev) {
	uint8_t status;
	if (read_data(dev, BMP280_REG_STATUS, &status, 1))
		return false;
	if (status & (1 << 3)) {
		return true;
	}
	return false;
}

/**
 * Compensation algorithm is taken from BMP280 datasheet.
 *
 * Return value is in degrees Celsius.
 */
static inline int32_t compensate_temperature(BMP280_HandleTypedef *dev, int32_t adc_temp,
		int32_t *fine_temp) {
	int32_t var1, var2;

	var1 = ((((adc_temp >> 3) - ((int32_t) dev->dig_T1 << 1)))
			* (int32_t) dev->dig_T2) >> 11;
	var2 = (((((adc_temp >> 4) - (int32_t) dev->dig_T1)
			* ((adc_temp >> 4) - (int32_t) dev->dig_T1)) >> 12)
			* (int32_t) dev->dig_T3) >> 14;

	*fine_temp = var1 + var2;
	return (*fine_temp * 5 + 128) >> 8;
}

/**
 * Compensation algorithm is taken from BMP280 datasheet.
 *
 * Return value is in Pa, 24 integer bits and 8 fractional bits.
 */
static inline uint32_t compensate_pressure(BMP280_HandleTypedef *dev, int32_t adc_press,
		int32_t fine_temp) {
	int64_t var1, var2, p;

	var1 = (int64_t) fine_temp - 128000;
	var2 = var1 * var1 * (int64_t) dev->dig_P6;
	var2 = var2 + ((var1 * (int64_t) dev->dig_P5) << 17);
	var2 = var2 + (((int64_t) dev->dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t) dev->dig_P3) >> 8)
			+ ((var1 * (int64_t) dev->dig_P2) << 12);
	var1 = (((int64_t) 1 << 47) + var1) * ((int64_t) dev->dig_P1) >> 33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}

	p = 1048576 - adc_press;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t) dev->dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t) dev->dig_P8 * p) >> 19;

	p = ((p + var1 + var2) >> 8) + ((int64_t) dev->dig_P7 << 4);
	return p;
}



bool bmp280_read_fixed(BMP280_HandleTypedef *dev, int32_t *temperature, uint32_t *pressure)
{
	int32_t adc_pressure;
	int32_t adc_temp;
	uint8_t data[8];



	// Need to read in one sequence to ensure they match.
	size_t size =  6;
	if (read_data(dev, 0xf7, data, size)) {
		return false;
	}

	adc_pressure = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	adc_temp = data[3] << 12 | data[4] << 4 | data[5] >> 4;

	int32_t fine_temp;
	*temperature = compensate_temperature(dev, adc_temp, &fine_temp);
	*pressure = compensate_pressure(dev, adc_pressure, fine_temp);



	return true;
}

bool bmp280_read_float(BMP280_HandleTypedef *dev, float *temperature, float *pressure)
{
	int32_t fixed_temperature;
	uint32_t fixed_pressure;

	if (bmp280_read_fixed(dev, &fixed_temperature, &fixed_pressure))
	{
		*temperature = (float) fixed_temperature / 100.0f;
		*pressure = (float) fixed_pressure / 25600.0f;

		return true;
	}

	return false;
}

void BMP280_Init(void)
{
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = &BMP280_I2C;

	while (!bmp280_init(&bmp280, &bmp280.params))
	{

		HAL_Delay(2000);
	}

}



#define FILTER_NUM	5
#define FILTER_A	0.1f

/*限幅平均滤波法*/
static void presssureFilter(float* in,float* out)
{
	static uint8_t i=0;
	static float filter_buf[FILTER_NUM]={0.0f};
	float filter_sum=0.0f;
	uint8_t cnt=0;
	float deta;

	if(filter_buf[i]==0.0f)
	{
		filter_buf[i]=*in;
		*out=*in;
		if(++i>=FILTER_NUM)
			i=0;
	}
	else
	{
		if(i)
			deta=*in-filter_buf[i-1];
		else
			deta=*in-filter_buf[FILTER_NUM-1];

		if(fabs(deta)<FILTER_A)
		{
			filter_buf[i]=*in;
			if(++i>=FILTER_NUM)
				i=0;
		}
		for(cnt=0;cnt<FILTER_NUM;cnt++)
		{
			filter_sum+=filter_buf[cnt];
		}
		*out=filter_sum /FILTER_NUM;
	}
}


#define CONST_PF 0.1902630958	                                               //(1/5.25588f) Pressure factor
#define FIX_TEMP 25				                                               // Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
								                                               // TLDR: Adjusting for temp changes does more harm than good.
/*
 * Converts pressure to altitude above sea level (ASL) in meters
*/
static float bmp280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/)
{
    if (*pressure>0)
    {
        return((FastPow((1015.7f/ *pressure),CONST_PF)-1.0f)*(FIX_TEMP+273.15f))/0.0065f;
    }
    else
    {
        return 0;
    }
}





void bmp280_get_data(float* temperature, float* pressure, float* asl)
{
   float pressure_raw;
   while (!bmp280_read_float(&bmp280, temperature, &pressure_raw))
   {
	   HAL_Delay(2000);
   }

   presssureFilter(&pressure_raw,pressure);

   *asl=bmp280PressureToAltitude(pressure);


}





