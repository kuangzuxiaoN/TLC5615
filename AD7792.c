#include "AD7792.h"  

void AD7792_Reset(void)			//AD7792 Reset
{
	uint8_t ResetTime=32;
	
	AD7792_SCK_1();	
	AD7792_CS_0();
	AD7792_DI_1();	
	
	while(ResetTime--) 			//to keep DIN=1 for 32 sclock to reset the part
	{
		Delay_us(1);
		AD7792_SCK_0();	
		Delay_us(1);
		AD7792_SCK_1();	
	}
	AD7792_CS_1();
	
	printf("AD7792 Reset\r\n");
}

/* 对于读写操作，当对选定寄存器的读写操作完成后，接口返回到通信寄存器执行写操作的状态 */
void AD7792_RegInit(void)
{
	unsigned char DataRead[3];
	
	AD7792_Reset();
	printf("AD7792 Register Initial!\r\n"); 
	
	AD7792_Write1Byte(0x10); 	//写使能，写配置寄存器write to Communication register.The next step is writing to Configuration register. 
//	AD7792_Write2Byte(0x1010);//单极性 输入允许0-2V5 缓冲模式 外部基准电压
	AD7792_Write2Byte(0x0010);//双极性 输入允许0-2V5 缓冲模式 外部基准电压
		
	AD7792_Write1Byte(0x08);	//写使能，write to Communication register.The next step is writing to Mode register. 
	AD7792_Write2Byte(0x200A);
	
	AD7792_Write1Byte(0x40);	//读使能，write to Communication register.The next step is to read from Status register.
	AD7792_ReadFromReg(DataRead, 1);	
	while((DataRead[0]&0x80)==0x80)			//wait for the end of convertion by polling the status register RDY bit 
	{
	  
		AD7792_Write1Byte(0x40); 
		AD7792_ReadFromReg(DataRead, 1);	
	}
	  
	AD7792_Write1Byte(0x58);//write to Communication register.The next step is to read from Data register.
	AD7792_ReadFromReg(DataRead, 2);	
	printf("Data : 0x%02x, 0x%02x\r\n",DataRead[0],DataRead[1]); 
}
	
void AD7792_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AD7792_SCK | RCC_AD7792_DIN | RCC_AD7792_DOUT | RCC_AD7792_CS, ENABLE);
	
	/* 配置几个推挽输出IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_AD7792_SCK;
	GPIO_Init(PORT_AD7792_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_AD7792_DIN;			/*对于主机而言为输入，对于从机而言为输出，因此此处配置成输出*/
	GPIO_Init(PORT_AD7792_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_AD7792_CS;
	GPIO_Init(PORT_AD7792_CS, &GPIO_InitStructure);
	
	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 设为输入口 */  	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_AD7792_DOUT;
	GPIO_Init(PORT_AD7792_DOUT, &GPIO_InitStructure);
	
	AD7792_SCK_1();	AD7792_CS_1();		/*空闲SCLK为1， CS为高*/
	AD7792_Reset();
	AD7792_RegInit();
} 

/*
*********************************************************************************************************
*	函 数 名: AD7792_Write1Byte
*	功能说明: 向SPI总线发送8个bit数据。 带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD7792_Write1Byte(uint8_t _data)
{
	uint8_t i;  
	unsigned char temp = 0x80;
	
	AD7792_CS_0();
	for(i = 0; i < 8; i++)
	{
		if ((_data & temp) == 0)
		{
			AD7792_DI_0();
		}
		else
		{
			AD7792_DI_1();
		}
		AD7792_SCK_0();
		Delay_us(1);
		AD7792_SCK_1();	
		Delay_us(1);	
		temp = temp >> 1;
	} 
	AD7792_CS_1();
}

void AD7792_Write2Byte(uint16_t _data)
{
	uint8_t i;  
	uint16_t temp = 0x8000;
	
	AD7792_CS_0();
	for(i = 0; i < 16; i++)
	{
		if ((_data & temp) == 0)
		{
			AD7792_DI_0();
		}
		else
		{
			AD7792_DI_1();
		}
		AD7792_SCK_0();
		Delay_us(1);
		AD7792_SCK_1();	
		Delay_us(1);	
		temp = temp >> 1;
	} 
	AD7792_CS_1();
}

/*
*********************************************************************************************************
*	函 数 名: AD7792_ReadFromReg
*	功能说明: 写入n个字节。带CS控制
*	形    参: *DataRead ：将要写入的数据数组，nByte ：数组长度
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7792_ReadFromReg(unsigned char *DataRead, unsigned char nByte) // nByte is the number of bytes which need to be read
{
	uint8_t i,j;
  unsigned char temp = 0;
			
	AD7792_DI_1();
	AD7792_CS_0();
	AD7792_DO_1();  

	for(i=0; i<nByte; i++)
	{
		for(j=0; j<8; j++)
		{
				AD7792_SCK_0();
				if(AD7792_DO_IS_LOW())
				{
					temp=temp<<1;
				}else
				{
					temp=temp<<1;
					temp=temp+0x01;
				}
				
				Delay_us(1);
				AD7792_SCK_1();	
				Delay_us(1);
		}
		DataRead[i]=temp; 
	}
	
	AD7792_CS_1();
}


uint16_t AD7792_ReadData(void)
{ 
	unsigned char DataRead[2]={0x0};   
	
	AD7792_Write1Byte(0x08);	//写使能，write to Communication register.The next step is writing to Mode register. 
	AD7792_Write2Byte(0x200A);
	
	AD7792_Write1Byte(0x40);	//读使能，write to Communication register.The next step is to read from Status register.
	AD7792_ReadFromReg(DataRead, 1);	
	while((DataRead[0]&0x80)==0x80)			//wait for the end of convertion by polling the status register RDY bit 
	{
	  
		AD7792_Write1Byte(0x40); 
		AD7792_ReadFromReg(DataRead, 1);	
	}
	  
	AD7792_Write1Byte(0x58);//读数据寄存器write to Communication register.The next step is to read from Data register.
	AD7792_ReadFromReg(DataRead, 2);	
	//printf("Data : 0x%02x, 0x%02x\r\n",DataRead[0],DataRead[1]); 
	
	return ((DataRead[0] << 8) | DataRead[1]); 
}








