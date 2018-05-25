#ifndef __AD7788_H__
#define __AD7788_H__	 
 
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h" 
#include "usart.h"	


#define RCC_AD7788_SCK 							RCC_APB2Periph_GPIOF
#define PORT_AD7788_SCK							GPIOF
#define PIN_AD7788_SCK							GPIO_Pin_0 
 
#define RCC_AD7788_DIN 							RCC_APB2Periph_GPIOF
#define PORT_AD7788_DIN							GPIOF
#define PIN_AD7788_DIN							GPIO_Pin_1

#define RCC_AD7788_DOUT 						RCC_APB2Periph_GPIOF
#define PORT_AD7788_DOUT						GPIOF
#define PIN_AD7788_DOUT							GPIO_Pin_2

#define RCC_AD7788_CS 							RCC_APB2Periph_GPIOF
#define PORT_AD7788_CS							GPIOF
#define PIN_AD7788_CS								GPIO_Pin_3 

#define AD7788_CS_0()								GPIO_ResetBits(PORT_AD7788_CS, PIN_AD7788_CS)
#define AD7788_CS_1()								GPIO_SetBits(PORT_AD7788_CS, PIN_AD7788_CS)

#define AD7788_SCK_0()							GPIO_ResetBits(PORT_AD7788_SCK, PIN_AD7788_SCK)
#define AD7788_SCK_1()							GPIO_SetBits(PORT_AD7788_SCK, PIN_AD7788_SCK)

#define AD7788_DI_0()								GPIO_ResetBits(PORT_AD7788_DIN, PIN_AD7788_DIN)
#define AD7788_DI_1()								GPIO_SetBits(PORT_AD7788_DIN, PIN_AD7788_DIN)

#define AD7788_DO_0()								GPIO_ResetBits(PORT_AD7788_DOUT, PIN_AD7788_DOUT)
#define AD7788_DO_1()								GPIO_SetBits(PORT_AD7788_DOUT, PIN_AD7788_DOUT)
 
#define AD7788_DO_IS_HIGH()					(GPIO_ReadInputDataBit(PORT_AD7788_DOUT, PIN_AD7788_DOUT) == Bit_SET)
#define AD7788_DO_IS_LOW()					(GPIO_ReadInputDataBit(PORT_AD7788_DOUT, PIN_AD7788_DOUT) == Bit_RESET) 

/* COMMUNICATIONS REGISTER */
#define AD7788_WRITE_ENABLE					(0x0 << 7)		//写使能，必须设置位
#define AD7788_REG_COMM							(0x0 << 4)		//8Bits					/* For writes */
#define AD7788_REG_STATUS						(0x0 << 4)		//8Bits					/* For reads */
#define AD7788_REG_MODE							(0x1 << 4)		//8Bits 
#define AD7788_REG_DATA							(0x3 << 4)		//16Bits
#define AD7788_READ_REG							(0x1 << 3)
#define AD7788_WRITE_REG						(0x0 << 3)
#define AD7788_CONTINUOUS_READ			(0x1 << 2)
#define AD7788_CH_AIN1P_AIN1N				0x0 					//差分输入					//通道选择
#define AD7788_CH_AIN1N_AIN1N				0x2						//单端输入（反相端）
#define AD7788_CH_AVDD_MONITOR			0x3 

/* STATUS REGISTER */   // Read only
#define AD7788_READY								(0x0 << 7)
#define AD7788_ERROR								(0x1 << 6)
#define AD7788_IDENTIFIER						(0x0 << 2)	//the bit is cleared automatically if the device is an AD7788 and it is set automatically if the device is an AD7789 

/* MODE REGISTER */
#define AD7788_MODE_SEL(x)					((x) << 6)
#define AD7788_MODE_CONTINUOUS			(0x0 << 6)		//连续转换模式
#define AD7788_MODE_SINGLE					(0x2 << 6)		//单次转换模式
#define AD7788_MODE_POWERDOWN				(0x3 << 6)		//掉电模式
#define AD7788_MODE_BIPOLAR					(0x0 << 2) 		//有极性输入（默认模式）
#define AD7788_MODE_UNIPOLAR				(0x1 << 2) 		//无极性输入 
  
void AD7788_RegInit(void);
void AD7788_Init(void);
void AD7788_WriteByte(uint8_t _data);  
void AD7788_ReadFromReg(uint8_t* dataBuf, uint8_t nByte); 
 
uint16_t AD7788_ReadData(void);

#endif


