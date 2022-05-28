




#include "bsp_u8g2.h"

u8g2_t u8g2; 





static uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成了*/
            break;
        case U8X8_MSG_DELAY_MILLI: /*延时函数*/
            //HAL_Delay(arg_int);
            break;
        case U8X8_MSG_GPIO_CS:
        	HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,arg_int);
            break;
        case U8X8_MSG_GPIO_DC: /*设置DC引脚,表明发送的是数据还是命令*/
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
        	//HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,arg_int);
            break;
    }
    return 1;

}




uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND: /*通过SPI发送arg_int个字节数据*/
			HAL_SPI_Transmit(&OLED_SPI_HANDLE,(uint8_t *)arg_ptr,arg_int,200);
			//HAL_SPI_Transmit_DMA(&OLED_SPI_HANDLE, (uint8_t *)arg_ptr, arg_int);
            break;
        case U8X8_MSG_BYTE_INIT: /*初始化函数*/
            break;
        case U8X8_MSG_BYTE_SET_DC: /*设置DC引脚,表明发送的是数据还是命令*/
			HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            break;
        default:
            return 0;
    }
    return 1;
}










void u8g2Init(u8g2_t *u8g2)
{
	OLED_RES_Clr();
	HAL_Delay(100);
	OLED_RES_Set();

	u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R3, u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay);
    u8g2_InitDisplay(u8g2);                                                                       
    u8g2_SetPowerSave(u8g2, 0);                                                                   
    u8g2_ClearBuffer(u8g2);
}
