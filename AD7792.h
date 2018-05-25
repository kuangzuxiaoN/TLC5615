#ifndef __AD7792_H__
#define __AD7792_H__	 
 
#include "stm32f10x.h"
#include "delay.h" 
#include "usart.h"	


#define RCC_AD7792_SCK 		RCC_APB2Periph_GPIOF
#define PORT_AD7792_SCK		GPIOF
#define PIN_AD7792_SCK		GPIO_Pin_1

#define RCC_AD7792_DIN 		RCC_APB2Periph_GPIOF
#define PORT_AD7792_DIN		GPIOF
#define PIN_AD7792_DIN		GPIO_Pin_2

#define RCC_AD7792_DOUT 	RCC_APB2Periph_GPIOF
#define PORT_AD7792_DOUT	GPIOF
#define PIN_AD7792_DOUT		GPIO_Pin_3

#define RCC_AD7792_CS 		RCC_APB2Periph_GPIOF
#define PORT_AD7792_CS		GPIOF
#define PIN_AD7792_CS			GPIO_Pin_0 

 
#define AD7792_CS_0()			GPIO_ResetBits(PORT_AD7792_CS, PIN_AD7792_CS)
#define AD7792_CS_1()			GPIO_SetBits(PORT_AD7792_CS, PIN_AD7792_CS)

#define AD7792_SCK_0()		GPIO_ResetBits(PORT_AD7792_SCK, PIN_AD7792_SCK)
#define AD7792_SCK_1()		GPIO_SetBits(PORT_AD7792_SCK, PIN_AD7792_SCK)

#define AD7792_DI_0()			GPIO_ResetBits(PORT_AD7792_DIN, PIN_AD7792_DIN)
#define AD7792_DI_1()			GPIO_SetBits(PORT_AD7792_DIN, PIN_AD7792_DIN)

#define AD7792_DO_0()			GPIO_ResetBits(PORT_AD7792_DOUT, PIN_AD7792_DOUT)
#define AD7792_DO_1()			GPIO_SetBits(PORT_AD7792_DOUT, PIN_AD7792_DOUT)

#define AD7792_DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_AD7792_DOUT, PIN_AD7792_DOUT) == Bit_SET)
#define AD7792_DO_IS_LOW()	(GPIO_ReadInputDataBit(PORT_AD7792_DOUT, PIN_AD7792_DOUT) == Bit_RESET)


void AD7792_Reset(void);
void AD7792_RegInit(void);
void AD7792_Init(void);
void AD7792_Write1Byte(uint8_t _data);
void AD7792_Write2Byte(uint16_t _data);
void AD7792_ReadFromReg(unsigned char *DataRead, unsigned char nByte);

uint16_t AD7792_ReadData(void);

#endif


