#include "data.h"


uint32_t tick = 0;// increase 1 every 1 ms in TIM IT ISR
uint32_t steps = 0;
Axis6_t axis6_raw,axis6_final;
Eula3_t eula3;		
uint8_t alarm[3]={16,22,30};


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	static uint8_t cnt;
	
	if(htim->Instance==TIM14)
  {
		tick++;

		if(++cnt==5)
		{
			cnt = 0;
			
			biquad_filter_apply(&axis6_raw,&axis6_final);
   
	    //imu_update(axis6_final,&eula3);
			imu_get_eula(axis6_final,&eula3);
			
			steps = get_steps(axis6_final);

		}
	}
}


/**
  * @brief  alarm reminder
  * @param  sTime: Time structure
  *         alarm: alarm array ptr
  * @retval none 
	* @date   2022-03-05
*/
void is_alarm_excute(RTC_TimeTypeDef s_time,uint8_t* alarm)
{
	static FlagStatus beep_flag = RESET;  //0£ºalm stop 1:alm apply
	static uint8_t delay_cnt;
	
  if(s_time.Hours ==alarm[0] && s_time.Minutes==alarm[1] && s_time.Seconds==alarm[2])
	{
		beep_flag = SET;
  	BEEP_ON();
	}

	if(beep_flag==SET)
	{
	  if(++delay_cnt==30)
		{
			delay_cnt = 0;
		  beep_flag = RESET;
      BEEP_OFF();
		}
	}
	
}  



/**
  * @brief  get data from sensors,periph
  * @param  data: data structure ptr
  * @retval none 
	* @date   2022-01-26
  */
void data_get(Data* data)
{
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc,10);    

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); 
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	is_alarm_excute(sTime,alarm);
  icm20602_get_axis6_data(&axis6_raw);
	eula3.yaw = 	hmc5883l_read();	
	imu_get_dir(&eula3, &(data->dir));

	
	data->hour = sTime.Hours;
  data->min  = sTime.Minutes;
	data->sec  = sTime.Seconds;
	data->date = sDate.Date;
	data->week = sDate.WeekDay;
	data->month = sDate.Month;
	data->year = sDate.Year;
 
	data->batt = (float)HAL_ADC_GetValue(&hadc)/4096*3.3*2;
	
	data->pitch = eula3.pitch;
	data->roll = eula3.roll;
	data->yaw = eula3.yaw;
  data->az = axis6_final.az;
  data->step = steps;
}


