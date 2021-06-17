# PCA9685_STM32HAL
移植Ardunio的舵机驱动板库

 - 仅需要添加这两个文件到工程即可；
 - 仅需要实现4个接口函数；

### 文件结构

|文件结构                                        |描述   |
|:------------------------------               |:----- |
|PWMServoDriver.c                        |实现PCA9685的驱动|
|PWMServoDriver.h                     |封装常用的一些操作|

### 接口实现
|接口                                        |功能描述   |
|:------------------------------               |:----- |
|static uint8_t PCA9685_read(uint8_t startAddress)                        |实现使用i2c从PCA9685寄存器中读取一个字节|
|static void PCA9685_write_onebyte(uint8_t startAddress, uint8_t data)                    |实现使用i2c往PCA9685寄存器中写入一个字节|
|static void PCA9685_write_bytes(uint8_t startAddress, uint8_t *data,uint8_t size)                   |实现使用i2c往PCA9685寄存器中写入多个字节|
|static void delay(uint16_t ms)                   |实现毫秒延时|

### 常用函数
|函数名称                                        |功能描述   |
|:------------------------------               |:----- |
|void PCA9685_Init(uint8_t addr,float freq);                     |初始化驱动板地址和pwm频率|
|void SetPWM(uint8_t num,uint16_t on,uint16_t off);                  |指定num通道，输出脉宽，on脉冲开始时间，off脉冲结束时间|

### 使用示例（驱动航模舵机）
``` c
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
   PCA9685_Init(PCA9685_I2C_ADDRESS,50); //初始化驱动板，并设置PWM频率为50HZ
   uint8_t servonum = 0;
   while(1)
   {
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        SetPWM(servonum, 0, pulselen); //设置PWM通道输出
      }
     HAL_Delay(500);
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
       SetPWM(servonum, 0, pulselen); //设置PWM通道输出
     }
      HAL_Delay(500);
      servonum++;
      if (servonum > 8) servonum = 0; // Testing the first 8 servo channels
   }
```

