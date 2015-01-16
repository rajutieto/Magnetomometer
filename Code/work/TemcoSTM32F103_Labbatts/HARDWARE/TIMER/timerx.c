#include "timerx.h"
#include "led.h"
#include "labatts.h"

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
//	if(TIM_GetFlagStatus(TIM3, TIM_IT_Update) == SET)
//	{
//		LED1 = !LED1;
//	}
	FGM3_ISR();
	TIM_ClearFlag(TIM3, TIM_IT_Update);	
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
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
	
	//Timer3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								//根据指定的参数初始化NVIC寄存器
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, DISABLE);
}


////通用定时器4中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器3!
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


//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//void TIM3_PWM_Init(u16 arr, u16 psc)
//{		 					 
//	//此部分需手动修改IO口设置
//	RCC->APB1ENR |= 1 << 1;		//TIM3时钟使能    
//	RCC->APB2ENR |= 1 << 3;		//使能PORTB时钟	
//	GPIOB->CRL &= 0XFF0FFFFF;	//PB5输出
//	GPIOB->CRL |= 0X00B00000;	//复用功能输出 	  	 
//	   
//	RCC->APB2ENR |= 1 << 0;		//开启辅助时钟	   
//	AFIO->MAPR &= 0XFFFFF3FF;	//清除MAPR的[11:10]
//	AFIO->MAPR |= 1 << 11;		//部分重映像,TIM3_CH2->PB5

//	TIM3->ARR = arr;			//设定计数器自动重装值 
//	TIM3->PSC = psc;			//预分频器不分频
//	
//	TIM3->CCMR1 |= 7 << 12;		//CH2 PWM2模式		 
//	TIM3->CCMR1 |= 1 << 11;		//CH2预装载使能	   
//	TIM3->CCER |= 1 << 4;		//OC2 输出使能	   
//	TIM3->CR1 = 0x0080;			//ARPE使能 
//	TIM3->CR1 |= 0x01;			//使能定时器3 											  
//}  	 

/////////////////////////////////////////////////////////////////////////////////////////
u32 uip_timer = 0;	//uip 计时器，每10ms增加1.
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			    
//	if(TIM6->SR & 0X0001)	//溢出中断
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) == SET)
	{
		uip_timer++;		//uip计时器增加1
	}
	
//	TIM6->SR &= ~(1 << 0);	//清除中断标志位 
	TIM_ClearFlag(TIM6, TIM_IT_Update);		
}

//基本定时器6中断初始化					  
//arr：自动重装值。		
//psc：时钟预分频数		 
//Tout= ((arr+1)*(psc+1))/Tclk；
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
	
	//Timer3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								//根据指定的参数初始化NVIC寄存器
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
//	RCC->APB1ENR |= 1 << 4;					//TIM6时钟使能    
// 	TIM6->ARR = arr;  						//设定计数器自动重装值 
//	TIM6->PSC = psc;  			 			//设置预分频器.
// 	TIM6->DIER |= 1 << 0;   				//允许更新中断				
// 	TIM6->CR1 |= 0x01;    					//使能定时器6
//	MY_NVIC_Init(0, 0, TIM6_IRQn, 2);		//抢占1，子优先级2，组2		
}
