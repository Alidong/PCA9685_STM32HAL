/*************************************************** 
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <PWMServoDriver.h>
#include "i2c.h"
#include "math.h"
// Set to true to print some debug messages, or false to disable them.
#define ENABLE_DEBUG_OUTPUT true
static uint8_t PCA9685_read(uint8_t startAddress);
static void PCA9685_write(uint8_t startAddress, uint8_t data);
static uint8_t PCA9685_adrr;
static void delay(uint16_t ms);
void PCA9685_Reset(void)
{
    PCA9685_write(PCA9685_MODE1,MODE1_RESTART);
    delay(10);
}

void PCA9685_Init(uint8_t addr)
{
    PCA9685_adrr=addr;
//    for(uint8_t add=0;add<0xFF;add++)
//    {
//      if(HAL_I2C_IsDeviceReady(&hi2c1,add,3,1000)==HAL_OK)
//        printf("is addr=%02x ready \r\n",add);
//    }
    PCA9685_Reset();
    SetPWMFreq(1000);
    delay(10);
}
void SetPWMFreq(float freq)
{
    uint8_t prescale,oldmode,newmode;
    // Range output modulation frequency is dependant on oscillator
    if (freq < 1)
      freq = 1;
    if (freq > 3500)
      freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)
    float prescaleval = ((FREQUENCY_OSCILLATOR / (freq * 4096.0)) + 0.5) - 1;
    if (prescaleval < PCA9685_PRESCALE_MIN)
      prescaleval = PCA9685_PRESCALE_MIN;
    if (prescaleval > PCA9685_PRESCALE_MAX)
      prescaleval = PCA9685_PRESCALE_MAX;
    prescale = (uint8_t)prescaleval;
    
    oldmode = PCA9685_read(PCA9685_MODE1);
    printf("oldmode=%02x\r\n",oldmode);
    newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
    PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
    PCA9685_write(PCA9685_PRESCALE, prescale); // set the prescaler
    printf("set the prescaler=%d\r\n",prescale);
    PCA9685_write(PCA9685_MODE1, oldmode);
    delay(5);
    // This sets the MODE1 register to turn on auto increment.
    PCA9685_write(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
    delay(10);
    printf("Read prescale=%d\r\n",PCA9685_read(PCA9685_PRESCALE));

}
void SetPWM(uint32_t num,uint32_t on,uint32_t off)
{
    uint8_t tx_buff[4];
    tx_buff[0]=on;
    tx_buff[1]=(on >> 8);
    tx_buff[2]=off;
    tx_buff[3]=(off >> 8);
    HAL_I2C_Mem_Write(&hi2c1,PCA9685_adrr,I2C_MEMADD_SIZE_8BIT,PCA9685_LED0_ON_L + 4 * num,tx_buff,4,10000);   
}

static uint8_t PCA9685_read(uint8_t startAddress) {
    //Send address to start reading from.
    uint8_t data;
    HAL_StatusTypeDef read_status=HAL_OK;
    read_status=HAL_I2C_Mem_Read(&hi2c1,PCA9685_adrr,I2C_MEMADD_SIZE_8BIT,startAddress,&data,1,10000);
    if(read_status==HAL_OK)
      return data;
    else
      printf("Read Err! %d\r\n",read_status);
}

static void PCA9685_writeonebyte(uint8_t startAddress, uint8_t data) {
    //Send address to start reading from. 
    HAL_StatusTypeDef write_status=HAL_OK;
    write_status=HAL_I2C_Mem_Write(&hi2c1,PCA9685_adrr,I2C_MEMADD_SIZE_8BIT,startAddress,&data,1,10000);
    if(write_status==HAL_OK)
      printf("Write ok!\r\n");
    else
      printf("Write Err! %d\r\n",write_status);
}
static void delay(uint16_t ms){
    HAL_Delay(ms);
}
