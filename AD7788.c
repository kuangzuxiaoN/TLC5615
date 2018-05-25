#include "AD7788.h"  

/* 对于读写操作，当对选定寄存器的读写操作完成后，接口返回到通信寄存器执行写操作的状态 */
void AD7788_RegInit(void)
{
	uint8_t wData=0x0; 
	 
	wData = AD7788_WRITE_ENABLE | AD7788_REG_MODE | AD7788_WRITE_REG | AD7788_CH_AIN1P_AIN1N;	//(0x10)写通信寄存器，next operations is write from Mode register.
	AD7788_WriteByte(wData); 
	wData = AD7788_MODE_CONTINUOUS | AD7788_MODE_BIPOLAR | (0x1<<1);			//(0x2)写模式寄存器, MR1 must 1
	AD7788_WriteByte(wData); 	//选择模式寄存器，有极性输入 
	 
}

void AD7788_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AD7788_SCK | RCC_AD7788_DIN | RCC_AD7788_DOUT | RCC_AD7788_CS, ENABLE);
	
	/* 配置几个推挽输出IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			/* 设为输出口 */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_AD7788_SCK;
	GPIO_Init(PORT_AD7788_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_AD7788_DIN;			
	GPIO_Init(PORT_AD7788_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_AD7788_CS;
	GPIO_Init(PORT_AD7788_CS, &GPIO_InitStructure);
	
	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 设为输入口 */  	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_AD7788_DOUT;
	GPIO_Init(PORT_AD7788_DOUT, &GPIO_InitStructure);
	  
	AD7788_SCK_1();	AD7788_CS_1();		/*空闲SCLK为1， CS为高*/
	
	AD7788_RegInit();
	
	printf("[%s][%d]: AD7788 Initial Success ! \r\n", __FUNCTION__, __LINE__);
} 

/*
*********************************************************************************************************
*	函 数 名: AD7788_Write8Bit
*	功能说明: 向SPI总线发送8个bit数据。 带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD7788_WriteByte(uint8_t _data)
{
	uint8_t i;   
	uint8_t temp=0x80; 
	
	AD7788_CS_0();
	
	for(i = 0; i < 8; i++)
	{ 
		if ((_data & temp) == 0) 
		{
			AD7788_DI_0();
		}
		else
		{
			AD7788_DI_1();
		}
		AD7788_SCK_0();		//低电平装载数据
//		Delay_us(1);
		AD7788_SCK_1();		//高电平输出
//		Delay_us(1);		
		temp = temp >> 1;
	}
	
	AD7788_CS_1(); 
}


void AD7788_ReadFromReg(uint8_t* dataBuf, uint8_t nByte)
{
	uint8_t i,j;  
	uint8_t temp = 0; 

	AD7788_DI_1();
	AD7788_CS_0();
	AD7788_DO_1();
	
	for(j=0; j<nByte; j++)
	{
		for (i = 0; i < 8; i++)
		{
			AD7788_SCK_0();
			if (AD7788_DO_IS_LOW())
			{
				temp=temp<<1;  
			}else{
				temp=temp<<1; 
				temp = temp + 0x01;
			}
//			Delay_us(1);
			AD7788_SCK_1();
//			Delay_us(1);	 
		}  
		dataBuf[j] = temp;
	}
	
	return ;
}
  

uint16_t AD7788_ReadData(void)
{ 
	uint8_t wData=0x0; 
	uint8_t dataBuf[2]={0x0};
  
	wData = AD7788_WRITE_ENABLE | AD7788_REG_STATUS | AD7788_READ_REG | AD7788_CH_AIN1P_AIN1N;	//(0x08)写通信寄存器，next operations is read from Status register.
	AD7788_WriteByte(wData);	
	AD7788_ReadFromReg(dataBuf, 1);																								//读状态寄存器 
	
	while((dataBuf[0] & 0x80) == 0x80)					//程序会死在这儿，直到DRDY位输入使能 （低电平使能）   
	{
		AD7788_WriteByte(wData);	
		AD7788_ReadFromReg(dataBuf, 1);																								//读状态寄存器 
	}  
	
	wData = AD7788_WRITE_ENABLE | AD7788_REG_DATA | AD7788_READ_REG | AD7788_CH_AIN1P_AIN1N;	//(0x38)写通信寄存器，next operations is read from Status register.
	AD7788_WriteByte(wData);
	 
	AD7788_CS_0(); 
	AD7788_ReadFromReg(dataBuf, 2);																								//读状态寄存器 
	AD7788_CS_1(); 
	
	return ((dataBuf[0] << 8) | dataBuf[1]);
}   








