#include "labatts.h"
#include "timerx.h"
#include "myiic.h"
#include "delay.h"
#include "stdio.h"
#include "modbus.h"
_FGM3_ fgm3;
_OPTO_ opto;
//u8 filter_times = 0;
u32 beer_counter = 0;
u8 opto_capture = 0;
void EXTI_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 );
	
	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	
	 
	
 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6 ); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3); 
	EXTI_InitStructure.EXTI_Line =EXTI_Line3 | EXTI_Line6  |  EXTI_Line8;
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	EXTI->IMR = (1 << 3);
}

//void Opto_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA, GPIO_Pin_3);
//}

void FGM3_Init(void)
{
	fgm3.status = 0;
	fgm3.ch1 = 0;
	fgm3.ch2 = 0;
	
//	Opto_Init();
	EXTI_init();
	
	FGM3_enable_read();
}

//u8 get_beer_counter(void)
//{
//	u8 cur_opto_stu;
//	static u8 pre_opto_stu = 0;
//	
//	cur_opto_stu = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
//	if(pre_opto_stu != cur_opto_stu)
//	{
//		if(cur_opto_stu == 0)
//		{
//			beer_counter++;
//		}
//		
//		pre_opto_stu = cur_opto_stu;
//	}
//	
//	return cur_opto_stu;
//}

void FGM3_enable_read(void)
{
	TIM_SetCounter(TIM3, 0);
	fgm3.status = 0;
	fgm3.ch1 = 0;
	fgm3.ch2 = 0;
//	filter_times = 0;
	EXTI->IMR |= (1 << 6)|(1 << 8);
//	EXTI->IMR = (1 << 6)|(1 << 8);
	TIM_Cmd(TIM3, ENABLE);
}

void FGM3_ISR(void)
{
//	filter_times++;
//	if(filter_times >= FILTER_TIMES)
//	{
		EXTI->IMR &= (~(1 << 6));         //???
		EXTI->IMR &= (~(1 << 8));
	  //  EXTI->IMR = 0;
		TIM_Cmd(TIM3, DISABLE);
		fgm3.status = 1;
//	}
}

void EXTI3_IRQHandler(void)
{
	if(EXTI->PR & (1 << 3))	//是3线的中断
	{    
		 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) 
		 { 
			 opto_capture = TRUE;
 		 }
		  else
		  {
 			opto_capture = FALSE; 
		  }
		EXTI->PR = (1 << 3);	//清除LINE3上的中断标志位
	}
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & (1 << 8))	//是8线的中断
	{    
		fgm3.ch2++;
		EXTI->PR  = (1 << 8);	//清除LINE8上的中断标志位
	}
	
	if(EXTI->PR & (1 << 6))	//是6线的中断
	{     
		fgm3.ch1++; 
		EXTI->PR  = (1 << 6);	//清除LINE6上的中断标志位
	}
	
	
}

void i2c_pic_start(void)
{
   // PIC chip requires a special double start condition in order
   // to initiate communication.  This is to insure the PIC does not 
   // read any false starts.
   // MDF 12/01/04
   
   // 1st start condition
//	 SDA_OFF;
//	_delay_loop_2(250);
//	 SCL_OFF;
//	_delay_loop_2(200);
	delay_us(100);
	SDA_OUT();		//sda线输出
	IIC_SDA = 0;
	delay_us(100);
	IIC_SCL = 0;
	delay_us(100);
   
	// reset bus
//	SDA_ON;
//	SCL_ON;
//	_delay_loop_2(200);
	IIC_SDA = 1;
	delay_us(100);
	IIC_SCL = 1;
	delay_us(100);
	// 2nd start condition
//	SDA_OFF;
//	_delay_loop_2(200);
//	SCL_OFF;
	IIC_SDA = 0;
	delay_us(100);
	IIC_SCL = 0;
}

void i2c_stop(void)
{
//	SDA_OFF;_delay_loop_2(5);
//	SCL_ON; _delay_loop_2(5);
//	SDA_ON;_delay_loop_2(5);
	SDA_OUT();		//sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 1;
	delay_us(4);
}

void GIVE_PIC_ACK(void)
{
   	u8 j = 0;
	// Wait until the data signal goes high
	SDA_IN();
	while(!READ_SDA)
	{
		j++;
		// If no clock, exit i2c_read routine
		if(j == 100)
		{
			return;
		}
	}

//	SDA_OFF;_delay_loop_2(10);
//	SCL_ON;

//	_delay_loop_2(100);

//	SCL_OFF; _delay_loop_2(10);
//	SDA_ON; 
//	_delay_loop_2(10);
	
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(100);
	IIC_SCL = 1;
	
	delay_us(100);
	
	IIC_SCL = 0;
	delay_us(100);
	IIC_SDA = 1;
	delay_us(100);
}



/*;************************************/
/*;send a 8-bit data to 12F675 */
void i2c_pic_write(unsigned char ch)
{
	unsigned char i = 8;
	SDA_OUT();
	do
	{
		delay_us(100);
		if(ch & 0x80)		//SDA = ( ch & 0x80 );
		{
//			SDA_ON;
			IIC_SDA = 1;
		}
		else
		{
//			SDA_OFF;
			IIC_SDA = 0;
		}

		ch <<= 1;
//		_delay_loop_2(300);
		delay_us(100);
      
//		SCL_ON;
//		_delay_loop_2(300); 
//		SCL_OFF;
		IIC_SCL = 1;
		delay_us(100);
		IIC_SCL = 0;  
     } while(--i != 0);
   
//	SDA_ON;
//	_delay_loop_2(20);
	delay_us(20);
	IIC_SDA = 1;
//	delay_us(20);
}

/*;**************************************/
/*;receive a 8-bit data from 12F675 */
unsigned int i2c_pic_read(void)
{
	unsigned char i;
	unsigned int data1 = 0;
	SDA_IN();
	for(i = 0; i < 16; i++)
	{
//		_delay_loop_2(300);
//		SCL_ON;
//		_delay_loop_2(300);
		delay_us(200);
		IIC_SCL = 1;
		delay_us(200);

//		if(SDA_IN)
		if(READ_SDA)
			data1 = ( data1 << 1 ) | 0x01;
		else
			data1 = ( data1 << 1 ) | 0x00;

//		SCL_OFF;
		IIC_SCL = 0;
	}
	return data1;
}

/*;**************************************/
/*;detect the ACK signal to 24Cxx*/   
// returns a 1 for no ACK.  returns a 0 for successful ACK      
unsigned int GET_ACK(void)
{ 
	unsigned char c = 1;
	unsigned int i = 0;
    
	SDA_IN();
	for(i = 0; i < 2000; i++)
	{
		c = READ_SDA;
		if(c == 0)
		{
			// if data line is low, pulse the clock.
//			SCL_ON;
//			_delay_loop_2(100);
//			SCL_OFF;
			IIC_SCL = 1;
			delay_us(100);
			IIC_SCL = 0;
			delay_us(100);
			
			return 0;
		}      
	}
//	SCL_OFF;
	IIC_SCL = 0;
	return 1;
}

u8 read_pic(void)
{	
	u16 checksum = 0;
	u16 value = 0;
	u16 sensitivity = 0;

	i2c_stop();
	i2c_pic_start();
  	i2c_pic_write(0xC1);

	delay_us(100);

	if(GET_ACK())
	{
		i2c_stop();
		printf("exit 1\r\n");
		return 0;
	}

//	_delay_loop_2(150);
	delay_us(5);
	value = i2c_pic_read();
	GIVE_PIC_ACK();
	
//	_delay_loop_2(150);
	delay_us(5);
	sensitivity = i2c_pic_read();
	GIVE_PIC_ACK();
	
//	_delay_loop_2(150);
	delay_us(5);
	checksum = i2c_pic_read();		//checksum
//	_delay_loop_2(250);
	delay_us(5);
	i2c_stop();

	if(checksum == ((value + sensitivity) & 0xffff))
	{
		opto.val = value;
		opto.sensitivity = sensitivity;
		printf("return: checksum OK\r\n");
		return 1;
	}
	else
	{
		printf("exit 3: checksum error\r\n");
		return 0;
	}
}

u8 write_pic(u16 sensitivity)
{
	u8 tempH, tempL;
	tempH = (u8)(sensitivity >> 8);
	tempL = (u8)sensitivity;
	
	i2c_stop();
	i2c_pic_start();
  	i2c_pic_write(0xC0);
	
	delay_us(5);
	if(GET_ACK())
	{
		i2c_stop();
		return 0xff;
	}
	
	i2c_pic_write(tempH);
	delay_us(5);
	if(GET_ACK())
	{
		i2c_stop();
		return 0xff;
	}
	
	i2c_pic_write(tempL);
	delay_us(5);
	if(GET_ACK())
	{
		i2c_stop();
		return 0xff;
	}
	
	i2c_pic_write((tempH + tempL) & 0xff);
	
	i2c_stop();
	return 0;
}
