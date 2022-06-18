# L-BAND V2.0  
### 演示视频 https://www.bilibili.com/video/BV1TL4y1K7E3?spm_id_from=333.999.0.0

2022-06-18 

* 删除`pwr_save_task`无效任务
* 使用-Os优化，减小代码体积，牺牲了一些运行速度，对性能影响不大
* 添加`environment_task`任务，负责获取气压、温度、海拔，以及`app_env`小程序负责这些量的显示

## 硬件  

1.主控: STM32G031F8P6 ARM Cortex-M0+ @64MHz  

2.IMU: LSM303DLHC  

3.气压计/温度计:BMP280  

4.电池:401120-80mAH锂聚合物电池  

5.显示器:0.96' SSD1306 OLED  


## 软件  
1.GUI：U8g2单色图形库  

2.功能：日历、实时时钟、温度、气压、海拔、计步、秒表、俯仰横滚角、指南针、游戏、电池电量

3.开发环境  STM32CubeIDE  

### 功耗：正常模式约8mA,待机模式约0.4mA，后续进一步降低


