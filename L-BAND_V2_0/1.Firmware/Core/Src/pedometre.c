
#include "pedometre.h"



#define FILTER_CNT		3

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SAMPLE_SIZE  10


#define ABS(a) (0 - (a)) > 0 ? (-(a)) : (a)
#define DYNAMIC_PRECISION     			30    	 /*动态精度*/



#define MOST_ACTIVE_NULL      			0      	 /*未找到最活跃轴*/
#define MOST_ACTIVE_X					1		 /*最活跃轴X*/
#define MOST_ACTIVE_Y					2        /*最活跃轴Y*/
#define MOST_ACTIVE_Z					3        /*最活跃轴Z*/

#define ACTIVE_PRECISION      			6       /*活跃轴最小变化值*/

typedef struct {
  short x;
  short y;
  short z;
}axis_info_t;

typedef struct {
	axis_info_t newmax;
	axis_info_t newmin;
	axis_info_t oldmax;
	axis_info_t oldmin;
}peak_value_t;


typedef struct filter_avg{
	axis_info_t info[FILTER_CNT];
	unsigned char count;
}filter_avg_t;

filter_avg_t  acc_data;



/*一个线性移位寄存器，用于过滤高频噪声*/
typedef struct slid_reg{
	axis_info_t new_sample;
	axis_info_t old_sample;
}slid_reg_t;

axis_info_t acc_sample;

peak_value_t acc_peak;

slid_reg_t  acc_slid;

unsigned char step_cnt = 0;





//在动态阈值结构体初始化
static void peak_value_init(peak_value_t *peak)
{
	peak->newmax.x = -1000;
	peak->newmax.y = -1000;
	peak->newmax.z = -1000;

	peak->newmin.x = 1000;
	peak->newmin.y = 1000;
	peak->newmin.z = 1000;

    peak->oldmax.x = 0;
    peak->oldmax.y = 0;
    peak->oldmax.z = 0;
    peak->oldmin.x = 0;
    peak->oldmin.y = 0;
	peak->oldmin.z = 0;
}


//在动态阈值结构体初始化时，一定要将max的值都赋值为最小值，min赋值为最大值，这样才有利于动态更新。
static void peak_update(peak_value_t *peak, axis_info_t *cur_sample)
{
  	static unsigned int sample_size = 0;
    sample_size ++;
	if (sample_size > SAMPLE_SIZE) {
		/*采样达到50个，更新一次*/
		sample_size = 1;
		peak->oldmax = peak->newmax;
		peak->oldmin = peak->newmin;
      	//初始化
      	peak->newmax.x = -1000;
      	peak->newmax.y = -1000;
      	peak->newmax.z = -1000;

		peak->newmin.x = 1000;
		peak->newmin.y = 1000;
	    peak->newmin.z = 1000;
      	//peak_value_init(peak);
	}
	peak->newmax.x = MAX(peak->newmax.x, cur_sample->x);
	peak->newmax.y = MAX(peak->newmax.y, cur_sample->y);
	peak->newmax.z = MAX(peak->newmax.z, cur_sample->z);

	peak->newmin.x = MIN(peak->newmin.x, cur_sample->x);
	peak->newmin.y = MIN(peak->newmin.y, cur_sample->y);
	peak->newmin.z = MIN(peak->newmin.z, cur_sample->z);

}



static char slid_update(slid_reg_t *slid, axis_info_t *cur_sample)
{
  	char res = 0;
  	if (ABS((cur_sample->x - slid->new_sample.x)) > DYNAMIC_PRECISION) {
		slid->old_sample.x = slid->new_sample.x;
		slid->new_sample.x = cur_sample->x;
		res = 1;
	} else {
		slid->old_sample.x = slid->new_sample.x;
	}
	if (ABS((cur_sample->y - slid->new_sample.y)) > DYNAMIC_PRECISION) {
		slid->old_sample.y = slid->new_sample.y;
		slid->new_sample.y = cur_sample->y;
		res = 1;
	} else {
		slid->old_sample.y = slid->new_sample.y;
	}

	if (ABS((cur_sample->z - slid->new_sample.z)) > DYNAMIC_PRECISION) {
		slid->old_sample.z = slid->new_sample.z;
		slid->new_sample.z = cur_sample->z;
		res = 1;
	} else {
		slid->old_sample.z = slid->new_sample.z;
	}
	return res;
}



/*判断当前最活跃轴*/
static char is_most_active(peak_value_t *peak)
{
	char res = MOST_ACTIVE_NULL;
	short x_change = ABS((peak->newmax.x - peak->newmin.x));
	short y_change = ABS((peak->newmax.y - peak->newmin.y));
	short z_change = ABS((peak->newmax.z - peak->newmin.z));

	if (x_change > y_change && x_change > z_change && x_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_X;
	} else if (y_change > x_change && y_change > z_change && y_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_Y;
	} else if (z_change > x_change && z_change > y_change && z_change >= ACTIVE_PRECISION) {
		res = MOST_ACTIVE_Z;
	}
	return res;
}

/*判断是否走步*/
static void detect_step(peak_value_t *peak, slid_reg_t *slid, axis_info_t *cur_sample)
{
	//static step_cnt = 0;
	char res = is_most_active(peak);
	switch (res) {
		case MOST_ACTIVE_NULL: {
			//fix
			break;
		}
		case MOST_ACTIVE_X: {
			short threshold_x = (peak->oldmax.x + peak->oldmin.x) / 2;
			//short shreshold_x =
			if (slid->old_sample.x > threshold_x && slid->new_sample.x < threshold_x) {
				step_cnt ++;
			}
			break;
		}
		case MOST_ACTIVE_Y: {
			short threshold_y = (peak->oldmax.y + peak->oldmin.y) / 2;

		//	short threshold_y = 1050;
			if (slid->old_sample.y > threshold_y && slid->new_sample.y < threshold_y) {
				step_cnt ++;
			}
			break;
		}
		case MOST_ACTIVE_Z: {
			short threshold_z = (peak->oldmax.z + peak->oldmin.z) / 2;
			if (slid->old_sample.z > threshold_z && slid->new_sample.z < threshold_z) {
				step_cnt ++;
			}
			break;
		}
		default:
			break;
	}
}

static void peak_value_init(peak_value_t *peak);




uint16_t step_count(Axis6_t axis6)
{

	acc_sample.x = (int16_t)(axis6.ax*1000.0f+0.5f);
	acc_sample.y = (int16_t)(axis6.ay*1000.0f+0.5f);
	acc_sample.z = (int16_t)(axis6.az*1000.0f+0.5f);

	peak_value_init(&acc_peak);

	peak_update(&acc_peak,  &acc_sample);

	slid_update(&acc_slid, &acc_sample);

	detect_step(&acc_peak, &acc_slid, &acc_sample);

	return step_cnt;
}
