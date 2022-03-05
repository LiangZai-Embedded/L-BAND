
#include "task.h"


uint32_t t = 0;

void task_data_get(void);
void task_ui(void);
void task_led(void);


#define TASK_NUM 3


task_t task[TASK_NUM] =    
{ 
 {0,5,5,task_data_get},
 {0,40,40,task_ui},
 {0,200,200,task_led}
};



															
															
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
    if (task[i].runflag != 0)  
    {
      task[i].excute();  
      task[i].runflag = 0;    
    }
  }
}

/**
  * @brief  data obtaining task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void task_data_get(void)
{
}
  
/**
  * @brief  ui task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void task_ui(void)
{
}

/**
  * @brief  led task
  * @param  none
  * @retval none
  * @date   2022-01-30
  */
void task_led(void)
{			
}







