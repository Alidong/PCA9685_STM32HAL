# PCA9685_STM32HAL
移植Ardunio的舵机驱动板库

 - 仅需要添加这两个文件到工程即可；
 - 仅需要实现三个接口位置；

### 文件结构

|文件结构                                        |描述   |
|:------------------------------               |:----- |
|PWMServoDriver.c                        |实现PCA9685的驱动|
|PWMServoDriver.h                     |封装常用的一些操作|

### 接口实现
|接口                                        |功能描述   |
|:------------------------------               |:----- |
|static uint8_t PCA9685_read(uint8_t startAddress)                        |实现使用i2c从PCA9685寄存器中读取一个字节|
|static void PCA9685_write(uint8_t startAddress, uint8_t data)                    |实现使用i2c往PCA9685寄存器中写入一个字节|
|static void delay(uint16_t ms)                   |实现毫秒延时|
