/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef enum
{
	MID = 0,
	FRWD,
	BKWD,
	LEFT,
	RIGHT
}dir_e;




typedef struct
{
  float pitch;
  float roll;
  float yaw;
}Euler3_t;


typedef struct
{
  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;
}Axis6_t;


typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
}Axis3_t;


typedef struct
{
	float  temperature;
	float  batt;
	float  pressure;

	uint8_t year;
    uint8_t month;
	uint8_t week;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;


	//ahrs
	float   roll;
    float   pitch;
    float   az;
	float   yaw;
    dir_e   dir;

	uint16_t step;



}Data;


#include "stm32g0xx_it.h"

#include "fastmath.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "task.h"

#include "u8g2.h"
#include "u8x8.h"

#include "i2c.h"
#include "rtc.h"
#include "adc.h"
#include "tim.h"

#include "imu.h"
#include "filter.h"
#include "pedometre.h"

#include "bmp280.h"
#include "lsm303dlhc.h"
#include "imu.h"
#include "filter.h"
#include "bsp_u8g2.h"





#include "ui.h"


#include "snake.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern void SystemClock_Config(void);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_DC_Pin GPIO_PIN_7
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_0
#define OLED_RES_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_3
#define OLED_CS_GPIO_Port GPIOA
#define MEMS_IT_Pin GPIO_PIN_4
#define MEMS_IT_GPIO_Port GPIOA
#define MEMS_IT_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */
#define INTERRUPT_TIM_HANDLE htim2
#define INTERRUPT_TIM_IRQN   TIM2_IRQn
#define COUNT_TIM_HANDLE     htim14

#define BEEP_OFF()		     HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
#define BEEP_ON()		     HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
