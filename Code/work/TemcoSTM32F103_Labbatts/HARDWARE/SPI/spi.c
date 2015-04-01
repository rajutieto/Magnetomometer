#include "spi.h"

//**** SPI1 ****************
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);	//PORTA��SPI1ʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;			//PA5-SCK, PA6-MISO, PA7-MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  							//PA5/6/7����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  				//PA5/6/7����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;									//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;								//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;									//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);											//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); 														//ʹ��SPI����
	SPI1_ReadWriteByte(0xff);//��������
	
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_256);
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//SCKƵ��=36M/4=9M
}

//SPI1�ٶ����ú���
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));		//���Ʒ�Χ
	SPI1->CR1 &= 0XFFC7; 
	SPI1->CR1 |= SPI_BaudRatePrescaler;									//����SPI1�ٶ�  
	SPI_Cmd(SPI1, ENABLE);												//SPI1�豸ʹ��	  
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u16 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	SPI_I2S_SendData(SPI1, TxData);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	return SPI_I2S_ReceiveData(SPI1);
}

//************ SPI2 ********************
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//PORTBʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);//SPI2ʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  							
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//��ʼ��GPIOA
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  				//PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;									//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;								//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;									//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);											//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); 														//ʹ��SPI����
	SPI2_ReadWriteByte(0xff);//��������
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_256);
// 	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);	//SCKƵ��=36M/4=9M
}

//SPI1�ٶ����ú���
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));		//���Ʒ�Χ
	SPI2->CR1 &= 0XFFC7; 
	SPI2->CR1 |= SPI_BaudRatePrescaler;									//����SPI2�ٶ�  
	SPI_Cmd(SPI2, ENABLE);												//SPI2�豸ʹ��	  
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	SPI_I2S_SendData(SPI2, TxData);
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	return SPI_I2S_ReceiveData(SPI2);
}

//**** SPI3 ****************
void SPI3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	//PORTB ʹ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE);//SPI3ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;			//PB3-SCK, PB4-MISO, PB5-MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  							//PB3/4/5����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//��ʼ��GPIOA
	GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);  				//PB3/4/5����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;									//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;								//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;									//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);											//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI3, ENABLE); 														//ʹ��SPI����
	SPI3_ReadWriteByte(0xff);//��������
	
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_256);
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_4);	//SCKƵ��=36M/4=9M
}

//SPI3�ٶ����ú���
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));		//���Ʒ�Χ
	SPI3->CR1 &= 0XFFC7; 
	SPI3->CR1 |= SPI_BaudRatePrescaler;									//����SPI3�ٶ�  
	SPI_Cmd(SPI3, ENABLE);												//SPI3�豸ʹ��	  
}

//SPI3 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u16 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	SPI_I2S_SendData(SPI3, TxData);
	
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 0xffff) return 0;
	};
	return SPI_I2S_ReceiveData(SPI3);
}

