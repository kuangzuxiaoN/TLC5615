#include "AD5660.h" 
 
void AD5660_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_SYNC, ENABLE);
	
	/* 配置几个推挽输出IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SYNC;
	GPIO_Init(PORT_SYNC, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = PIN_DIN;			/*对于主机而言为输入，对于从机而言为输出，因此此处配置成输出*/
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);
 
	SCK_1();
	SYNC_1();
	
	printf("AD5660_Init! \r\n");
} 

/*
*********************************************************************************************************
*	函 数 名: AD5660_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带SYNC控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD5660_SendBytes(uint8_t* dataBuf, uint8_t count)
{
	uint8_t i,j; 
	uint8_t wData=0; 
	  
	SYNC_1();
//	Delay_us(1);
	SYNC_0();				//同步信号由高到低表示数据开始
	 
	for(j = 0; j < count; j++)
	{
		wData = dataBuf[j];  
		for(i = 0; i < 8; i++)
		{
			SCK_1();			//上升沿取得数据位
			if (0x80 == (wData & 0x80))
			{
				DI_1();
			}
			else
			{
				DI_0();
			}
//			Delay_us(1);
			SCK_0();			//下降沿发送出去
//			Delay_us(1);		
			wData <<= 1;			
		}
	
	} 
	SYNC_1(); 
}






