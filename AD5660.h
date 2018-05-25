#ifndef __AD5660_H__
#define __AD5660_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h" 
#include "led.h"
#include "dac.h"

#define RCC_SCK 	RCC_APB2Periph_GPIOF
#define PORT_SCK	GPIOF
#define PIN_SCK		GPIO_Pin_8

#define RCC_DIN 	RCC_APB2Periph_GPIOF
#define PORT_DIN	GPIOF
#define PIN_DIN		GPIO_Pin_6 

#define RCC_SYNC 		RCC_APB2Periph_GPIOF
#define PORT_SYNC		GPIOF
#define PIN_SYNC		GPIO_Pin_10 

#define SYNC_0()		GPIO_ResetBits(PORT_SYNC, PIN_SYNC)
#define SYNC_1()		GPIO_SetBits(PORT_SYNC, PIN_SYNC)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)
  		
 
void AD5660_Init(void);
void AD5660_SendBytes(uint8_t* dataBuf, uint8_t count);


#endif
 









































