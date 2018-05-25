#ifndef __TLC5615_H__
#define __TLC5615_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h" 
#include "led.h"

/* 定义GPIO端口 */
#define RCC_SCK 	RCC_APB2Periph_GPIOF
#define PORT_SCK	GPIOF
#define PIN_SCK		GPIO_Pin_1

#define RCC_DIN 	RCC_APB2Periph_GPIOF
#define PORT_DIN	GPIOF
#define PIN_DIN		GPIO_Pin_0

#define RCC_DOUT 	RCC_APB2Periph_GPIOF
#define PORT_DOUT	GPIOF
#define PIN_DOUT	GPIO_Pin_3

#define RCC_CS 		RCC_APB2Periph_GPIOF
#define PORT_CS		GPIOF
#define PIN_CS		GPIO_Pin_5 


/* 定义口线置0和置1的宏 */
#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)

void tlc5615_Init(void);
void tlc5615_Send8Bit(uint8_t _data);
void tlc5615_WriteByte(uint8_t _data);
void tlc5615_Write2Byte(uint16_t _data);
void tlc5615_Send12Bit(uint16_t _data);
void generate_Wave(void);


#endif
 









































