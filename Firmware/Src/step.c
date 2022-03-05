#include "step.h"



#define MAX(a,b) ((a) > (b) ? (a) : (b)) 
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SAMPLE_SIZE   50
#define MOST_ACTIVE_NULL    0    /*未找到最活跃轴*/
#define MOST_ACTIVE_X				1		 /*最活跃轴X*/	
#define MOST_ACTIVE_Y				2    /*最活跃轴Y*/	
#define MOST_ACTIVE_Z				3    /*最活跃轴Z*/	
 
#define ACTIVE_PRECISION    10    /*活跃轴最小变化值*/
#define ABS(a) (0 - (a)) > 0 ? (-(a)) : (a)
#define DYNAMIC_PRECISION     			30     	 /*动态精度*/

typedef struct {
	int16_t newmax;
	int16_t newmin;
	int16_t oldmax;
	int16_t oldmin;
}peak_value_t;
peak_value_t peak;

void peak_value_init(peak_value_t *peak)
{
  peak->newmax = -20.0f;
 
	peak->newmin = 20.0f;

	peak->oldmax = -20.0f;

	peak->oldmin = 20.0f;

}

	//在动态阈值结构体初始化时，一定要将max的值都赋值为最小值，min赋值为最大值，这样才有利于动态更新。
static void peak_update(peak_value_t *peak, int16_t cur_sample)
{
  	static uint8_t sample_size = 0;
	static uint8_t start_flag = 0;
    sample_size ++;
	 if (sample_size > SAMPLE_SIZE) {
		/*采样达到50个，更新一次*/
		sample_size = 1;
		peak->oldmax = peak->newmax;
		peak->oldmin = peak->newmin;
    
   if(start_flag==0)		 //初始化
	 {
		 peak_value_init(peak);
		 start_flag = 1;
	 }
	}
	peak->newmax = MAX(peak->newmax, cur_sample);
	
	peak->newmin = MIN(peak->newmin, cur_sample);
	
}









 
/*一个线性移位寄存器，用于过滤高频噪声*/
typedef struct slid_reg{
	int16_t new_sample;
	int16_t old_sample;
}slid_reg_t;
slid_reg_t slid;

static void slid_update(slid_reg_t *slid, int16_t cur_sample)
{
  slid->old_sample = slid->new_sample;
	
	if (ABS((cur_sample - slid->new_sample)) > DYNAMIC_PRECISION) 
		slid->new_sample = cur_sample;
}


 
/*判断是否走步*/
static uint32_t detect_step(peak_value_t *peak, slid_reg_t *slid,int16_t cur_sample)
{
	static uint32_t step_cnt = 0;
	
	int16_t threshold = (peak->oldmax + peak->oldmin) / 2;
	if (slid->old_sample > threshold && slid->new_sample < threshold) 
	  step_cnt ++;
			

	
	return step_cnt;
}







/**
  * @brief  get step data
  * @param  axis6 6-axis raw data
  * @retval steps
  * @date   2022-02-07
  */
uint32_t get_steps(Axis6_t axis6)
{
	int16_t acc = (int16_t)(axis6.ay*1000.0f+0.5f);

	peak_update(&peak,acc);
	
	slid_update(&slid,acc);
	
	return detect_step(&peak,&slid,acc);
}
