#include "tlc5615.h" 

/*TLC5615��SPIͨ��ģʽΪMode0 : CPOL = 0, CPHA = 0*/

void tlc5615_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_DOUT | RCC_CS, ENABLE);
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;			/*������������Ϊ���룬���ڴӻ�����Ϊ�������˴˴����ó����*/
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	
	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);
	
	SCK_0();	CS_1();		/*����SCLKΪ0*/
	
} 

/*
*********************************************************************************************************
*	�� �� ��: tlc5615_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
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
*	�� �� ��: tlc5615_WriteByte
*	����˵��: д��1���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
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
*	�� �� ��: tlc5615_Write2Byte
*	����˵��: д��2���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
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
*	�� �� ��: tlc5615_Send12Bit
*	����˵��: ��SPI���߷���12��bit���ݡ� ��CS���ơ�
*	��    ��: _data : ���ݣ���Χ 0 ~ 1024
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void tlc5615_Send12Bit(uint16_t _data)	
{
	uint8_t i;

	_data = _data << 6;		/*����6λ�󣬴�ʱ���λΪMSB*/
	
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
		
		for(n=0; n<10; n++)	 	//	10������
		{
		   tlc5615_Send12Bit((uint16_t)1);
		   delay_ms(10);
		   tlc5615_Send12Bit((uint16_t)wave_Data);  //2.48
		   delay_ms(10);
		}

		for(n=0; n<10; n++)		// 10�����ǲ�
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









































