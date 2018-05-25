#include "tlc5615.h" 

/*TLC5615的SPI通信模式为Mode0 : CPOL = 0, CPHA = 0*/

void tlc5615_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_DOUT | RCC_CS, ENABLE);
	
	/* 配置几个推挽输出IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;			/*对于主机而言为输入，对于从机而言为输出，因此此处配置成输出*/
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	
	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);
	
	SCK_0();	CS_1();		/*空闲SCLK为0*/
	
} 

/*
*********************************************************************************************************
*	函 数 名: tlc5615_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/ 
void tlc5615_Send8Bit(uint8_t _data)
{
	uint8_t i; 
	
	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_1();
		_data <<= 1;
		Delay_us(1);
		SCK_0();	
		Delay_us(1);		
	}
}

/*
*********************************************************************************************************
*	函 数 名: tlc5615_WriteByte
*	功能说明: 写入1个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void tlc5615_WriteByte(uint8_t _data)
{
	CS_0();
	tlc5615_Send8Bit(_data);
	CS_1();
}

/*
*********************************************************************************************************
*	函 数 名: tlc5615_Write2Byte
*	功能说明: 写入2个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void tlc5615_Write2Byte(uint16_t _data)
{
	CS_0(); 
	tlc5615_Send8Bit((_data >> 8) & 0xFF);
	tlc5615_Send8Bit(_data);
	CS_1();
} 

/*
*********************************************************************************************************
*	函 数 名: tlc5615_Send12Bit
*	功能说明: 向SPI总线发送12个bit数据。 带CS控制。
*	形    参: _data : 数据，范围 0 ~ 1024
*	返 回 值: 无
*********************************************************************************************************
*/ 
void tlc5615_Send12Bit(uint16_t _data)	
{
	uint8_t i;

	_data = _data << 6;		/*左移6位后，此时最高位为MSB*/
	
	CS_0();
	
	for(i = 0; i < 12; i++)
	{
		if (_data & 0x8000)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_1();
		_data <<= 1;
		Delay_us(1);
		SCK_0();	
		Delay_us(1);	
	}
	
	CS_1();
}  

void generate_Wave(void)
{
	uint16_t n=0, j=0;
	uint16_t wave_Data = 500;
	
	while(1)
	{
		
		for(n=0; n<10; n++)	 	//	10个方波
		{
		   tlc5615_Send12Bit((uint16_t)1);
		   delay_ms(10);
		   tlc5615_Send12Bit((uint16_t)wave_Data);  //2.48
		   delay_ms(10);
		}

		for(n=0; n<10; n++)		// 10个三角波
		{
			while(j<500)
			{
			   tlc5615_Send12Bit((uint16_t)j);
			   j+=5;
			}
			while(j>1)
			{
				tlc5615_Send12Bit((uint16_t)j);
				j-=5;
			} 
		}
		LED2_REV;
	}

}









































