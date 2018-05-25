#include "AD5660.h" 
 
void AD5660_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_SYNC, ENABLE);
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SYNC;
	GPIO_Init(PORT_SYNC, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = PIN_DIN;			/*������������Ϊ���룬���ڴӻ�����Ϊ�������˴˴����ó����*/
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);
 
	SCK_1();
	SYNC_1();
	
	printf("AD5660_Init! \r\n");
} 

/*
*********************************************************************************************************
*	�� �� ��: AD5660_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����SYNC���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD5660_SendBytes(uint8_t* dataBuf, uint8_t count)
{
	uint8_t i,j; 
	uint8_t wData=0; 
	  
	SYNC_1();
//	Delay_us(1);
	SYNC_0();				//ͬ���ź��ɸߵ��ͱ�ʾ���ݿ�ʼ
	 
	for(j = 0; j < count; j++)
	{
		wData = dataBuf[j];  
		for(i = 0; i < 8; i++)
		{
			SCK_1();			//������ȡ������λ
			if (0x80 == (wData & 0x80))
			{
				DI_1();
			}
			else
			{
				DI_0();
			}
//			Delay_us(1);
			SCK_0();			//�½��ط��ͳ�ȥ
//			Delay_us(1);		
			wData <<= 1;			
		}
	
	} 
	SYNC_1(); 
}






