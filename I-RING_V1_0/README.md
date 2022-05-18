# I-RING V1.0  
### 演示视频 https://www.bilibili.com/video/BV1Ca411h7yj?spm_id_from=333.999.0.0  


2022-05-18  
* 解决了秒表LAP和SPL模式下时光倒流的问题  
* 解决了IIR滤波算法的计算步骤的错误


2022-04-13  
* 使用库STM32Cube_FW_F0_V1.11.0 ->ST官网下载  
* 删除进入app需 要shake的多余动作  
* 更改命名 eula->euler  
* app函数取消指针类型参数 app_xxx(Data* data)->app_xxx(Data data)  
* 由于精度原因，取消计步器，后续使用MPU9250 DMP硬件计步方法  

2022-03-07  
* 更新蜂鸣器封装，修改布线

## 硬件  

1.主控: STM32F030K6T6 ARM Cortex-M0@48MHz  

2.imu单元: ICM20602六轴传感器 HMC5883L磁力计  

3.充电芯片:TP4056  

4.充电接口:Micro USB  

5.显示器:SSD1306 I2C OLED  

## 软件  
1.GUI  u8g2单色图形库  

2.功能：电池电量，RTC,计步，秒表，闹铃，俯仰横滚角，指南针，游戏  

### 后续实现低功耗，测温，气压显示等 


