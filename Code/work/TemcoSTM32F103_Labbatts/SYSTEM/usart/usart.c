
#include "usart.h"	  
#include "modbus.h"

//////////////////////////////////////////////////////////////////////////////////
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;



//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
//	while((USART1->SR & 0X40) == 0);//ѭ������,ֱ���������   
//	USART1->DR = (u8)ch;
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, ch);     
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}

int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0;       //����״̬���	  



//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA10
	
	//RS485_TXEN	PA.8 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               //PA.8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //��ͨ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                  //��ʼ��PA8
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;					//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); 					//��ʼ������

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				//�����ж�
    USART_Cmd(USART1, ENABLE);                    				//ʹ�ܴ��� 
}

//void uart2_init(u32 bound)
//{
//    //GPIO�˿�����
//    GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 //REMAP
//	GPIO_PinRemapConfig (GPIO_Remap_USART2 ,ENABLE);
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);	//ʹ��USART1��GPIOAʱ��
// 	USART_DeInit(USART2);  //��λ��2
//	
//	//USART2_TX   PD.5
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//PD.5
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
//    GPIO_Init(GPIOD, &GPIO_InitStructure);					//��ʼ��PA2
//   
//    //USART2_RX	  PD.6
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PD.6
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
//    GPIO_Init(GPIOD, &GPIO_InitStructure);					//��ʼ��PA3
//	
//	
//	
//	
//	//RS485_TXEN	PD.7
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//PA.8
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//��ͨ�������
////	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA8
////	GPIO_SetBits(GPIOA, GPIO_Pin_8);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOD, GPIO_Pin_7);
//	
//	//Usart1 NVIC ����
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);								//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//  
//	//USART ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;					//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
//    USART_Init(USART2, &USART_InitStructure); 					//��ʼ������

//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				//�����ж�
//    USART_Cmd(USART2, ENABLE);                    				//ʹ�ܴ��� 
//}

 
  
#endif
