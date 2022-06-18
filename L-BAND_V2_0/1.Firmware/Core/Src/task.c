
#include "task.h"

Data data;
uint32_t tick = 0;// increase 1 every 1 ms in TIM IT ISR
uint8_t alarm[3]={20,28,20};
Axis6_t axis6_filter;

void ui_task(void);
void stepcount_task(void);
void batt_task(void);
void imu_task(void);
void rtc_task(void);
void environment_task(void);

#define TASK_NUM 6


task_t task[TASK_NUM] =    
{ 
 {0,1,1,imu_task},                //8ms
 {0,1,1,ui_task},                 //8ms
 {0,5,5,stepcount_task},          //40ms
 {0,125,125,batt_task},           //1000ms
 {0,1,1,rtc_task},                //8ms
 {0,125,125,environment_task}     //1000ms
};

/**
  * @brief  alarm reminder
  * @param  sTime: Time structure
  *         alarm: alarm array ptr
  * @retval none
	* @date   2022-03-05
*/
void is_alarm_excute(RTC_TimeTypeDef s_time,uint8_t* alarm)
{
	static FlagStatus beep_flag = RESET;


   if(s_time.Hours ==alarm[0] && s_time.Minutes==alarm[1] && s_time.Seconds==alarm[2])
	   beep_flag = SET;
   else
	   beep_flag = RESET;



	if(beep_flag==SET)
	{
		BEEP_ON();
	}
	else
	{
		BEEP_OFF();
	}


}




/**
  * @brief  excute environment task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void environment_task(void)
{
	bmp280_get_data(&data.temperature, &data.pressure, &data.asl);

	data.asl += 1;
}
/**
  * @brief  excute battary task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void batt_task(void)
{
	HAL_ADC_Start(&hadc1);
	while(HAL_OK!=HAL_ADC_PollForConversion(&hadc1,10)){}
	  data.batt = (float)HAL_ADC_GetValue(&hadc1)/4096*3.3*2;
}

/**
  * @brief  excute imu task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void imu_task(void)
{
  Euler3_t euler3;
  float buf_A[3],buf_M[3];
  Axis6_t axis6_raw;

  LSM303DLHC_AccReadXYZ(buf_A);
  LSM303DLHC_MagReadXYZ(buf_M);


  axis6_raw.ax = -buf_A[0];
  axis6_raw.ay = buf_A[1];
  axis6_raw.az = -buf_A[2];


  biquad_filter_apply(&axis6_raw, &axis6_filter);
  imu_get_euler(axis6_filter, &euler3);
  imu_get_yaw(&euler3,buf_M);
  imu_get_dir(&euler3, &(data.dir));


  data.pitch = euler3.pitch;
  data.roll = euler3.roll;
  data.yaw = euler3.yaw;
  data.az = axis6_filter.az;
}

/**
  * @brief  excute rtc task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void rtc_task(void)
{
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	is_alarm_excute(sTime,alarm);
	data.hour = sTime.Hours;
	data.min  = sTime.Minutes;
    data.sec  = sTime.Seconds;
	data.date = sDate.Date;
	data.week = sDate.WeekDay;
	data.month = sDate.Month;
	data.year = sDate.Year;
}


  
/**
  * @brief  excute GUI task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void ui_task(void)
{
    ui_main(data);
}



/**
  * @brief  excute stepcounter task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void stepcount_task(void)
{
	data.step = step_count(axis6_filter);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	tick++;
	if(tick%8==0)
	{
		for(uint8_t i = 0;i<TASK_NUM;i++)
		{
			if(--task[i].cnt==0)
			{
				task[i].cnt = task[i].reload;
				task[i].runflag = SET;
			}
		}
	}
}

















/**
  * @brief  task schedule
  * @param  none
  * @retval none
  * @date   2021-03-04
  */
void task_scheduler(void)
{
  for (uint8_t i = 0;i < TASK_NUM;i++)
  {
    if (task[i].runflag == SET)
    {

      task[i].excute();
      task[i].runflag = RESET;
    }
  }
}



