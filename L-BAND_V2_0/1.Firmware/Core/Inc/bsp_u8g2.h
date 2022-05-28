#ifndef _BSP_U8G2_H
#define _BSP_U8G2_H



#include "main.h"
#include "spi.h"
#include "u8g2.h"
#include "u8x8.h"

#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)

#define OLED_SPI_HANDLE hspi1

extern u8g2_t u8g2; 

void   u8g2Init(u8g2_t *u8g2);



#endif
