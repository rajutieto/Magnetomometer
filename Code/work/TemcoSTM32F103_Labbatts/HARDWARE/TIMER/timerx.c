#include "timerx.h"
#include "led.h"
#include "labatts.h"

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
//	if(TIM_GetFlagStatus(TIM3, TIM_IT_Update) == SET)
//	{
//		LED1 = !LED1;
//	}
	FGM3_ISR();
	TIM_ClearFlag(TIM3, TIM_IT_Update);	
}

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//Timer3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, DISABLE);
}


////ͨ�ö�ʱ��4�жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////����ʹ�õ��Ƕ�ʱ��3!
//void TIM4_Int_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_ICInitTypeDef TIM4_ICInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	
//	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM4_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
//	
//	TIM_Cmd(TIM4, DISABLE);
//}


//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//void TIM3_PWM_Init(u16 arr, u16 psc)
//{		 					 
//	//�˲������ֶ��޸�IO������
//	RCC->APB1ENR |= 1 << 1;		//TIM3ʱ��ʹ��    
//	RCC->APB2ENR |= 1 << 3;		//ʹ��PORTBʱ��	
//	GPIOB->CRL &= 0XFF0FFFFF;	//PB5���
//	GPIOB->CRL |= 0X00B00000;	//���ù������ 	  	 
//	   
//	RCC->APB2ENR |= 1 << 0;		//��������ʱ��	   
//	AFIO->MAPR &= 0XFFFFF3FF;	//���MAPR��[11:10]
//	AFIO->MAPR |= 1 << 11;		//������ӳ��,TIM3_CH2->PB5

//	TIM3->ARR = arr;			//�趨�������Զ���װֵ 
//	TIM3->PSC = psc;			//Ԥ��Ƶ������Ƶ
//	
//	TIM3->CCMR1 |= 7 << 12;		//CH2 PWM2ģʽ		 
//	TIM3->CCMR1 |= 1 << 11;		//CH2Ԥװ��ʹ��	   
//	TIM3->CCER |= 1 << 4;		//OC2 ���ʹ��	   
//	TIM3->CR1 = 0x0080;			//ARPEʹ�� 
//	TIM3->CR1 |= 0x01;			//ʹ�ܶ�ʱ��3 											  
//}  	 

/////////////////////////////////////////////////////////////////////////////////////////
u32 uip_timer = 0;	//uip ��ʱ����ÿ10ms����1.
//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		    		  			    
//	if(TIM6->SR & 0X0001)	//����ж�
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) == SET)
	{
		uip_timer++;		//uip��ʱ������1
	}
	
//	TIM6->SR &= ~(1 << 0);	//����жϱ�־λ 
	TIM_ClearFlag(TIM6, TIM_IT_Update);		
}

//������ʱ��6�жϳ�ʼ��					  
//arr���Զ���װֵ��		
//psc��ʱ��Ԥ��Ƶ��		 
//Tout= ((arr+1)*(psc+1))/Tclk��
void TIM6_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	//Timer3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
//	RCC->APB1ENR |= 1 << 4;					//TIM6ʱ��ʹ��    
// 	TIM6->ARR = arr;  						//�趨�������Զ���װֵ 
//	TIM6->PSC = psc;  			 			//����Ԥ��Ƶ��.
// 	TIM6->DIER |= 1 << 0;   				//��������ж�				
// 	TIM6->CR1 |= 0x01;    					//ʹ�ܶ�ʱ��6
//	MY_NVIC_Init(0, 0, TIM6_IRQn, 2);		//��ռ1�������ȼ�2����2		
}
