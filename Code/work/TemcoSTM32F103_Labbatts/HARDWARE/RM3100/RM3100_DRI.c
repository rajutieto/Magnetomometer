#include "RM3100_DRI.h"
#include "delay.h"
//IO方向设置
//#define SDA_IN()	{GPIOC->CRL &= 0X0FFFFFFF; GPIOC->CRL |= ((u32)8 << 28);} 

//IO操作函数	
#define SPI_MISO              PEin(0)
#define SPI_SCK               PEout(1) 
#define SPI_MOSI              PEout(2) 

#define SPI_CS0               PEout(4) 
#define SPI_CS1               PEout(6)

//#define SPI_STATE0            PEout(3) 
//#define SPI_STATE1            PEout(5) 

static void ENABLE_PNI_Sensor(u8 sensor)
{
	if(sensor == PNI_SENSOR0)
	{
		SPI_CS0 = 0; 
	}
	else if(sensor == PNI_SENSOR1)
	{ 
		SPI_CS1 = 0;
	}
}
static void DISABLE_PNI_Sensor(u8 sensor)
{
	if(sensor == PNI_SENSOR0)
	{
		SPI_CS0 = 1; 
	}
	else if(sensor == PNI_SENSOR1)
	{ 
		SPI_CS1 = 1;
	} 
}
  
//初始化SPI
void PNI_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOE时钟
	//output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6);
	//input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
	
	//interrupt 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_5 );
/*	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5 );  
	
	EXTI_InitStructure.EXTI_Line =EXTI_Line3; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line =EXTI_Line5; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
 
 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
*/	
 
}
   
static u8 spi_read()
{
	u8 buf_in = 0 ;
	u8 i ; 
	SPI_SCK = 0;//output_low(SPI_SCK);
	delay_us(5);
	for(i=0; i<8;i++)
	{
		SPI_SCK = 1;//output_HIGH(SPI_SCK);
		delay_us(5);
		if(SPI_MISO) buf_in |= (0x80>>i);
		SPI_SCK = 0;//output_low(SPI_SCK); 
		delay_us(5);
	} 
	return buf_in ;
}

static void spi_write(u8 buf_out)
{
	u8 i = 8;
	u8 temp ;
	SPI_SCK = 0;//output_LOW(SPI_SCK);
	delay_us(5);
	do
	{    
		temp = ( buf_out & 0x80 );
		if(temp >0 ) SPI_MOSI = 1;//output_high(SPI_MOSI);
		else         SPI_MOSI = 0;//output_low(SPI_MOSI);
		delay_us(5);
		SPI_SCK = 1;//output_HIGH(SPI_SCK);
		delay_us(5);
		SPI_SCK = 0;//output_LOW(SPI_SCK);
		buf_out <<=1;
		delay_us(5);
	} while( --i != 0 );
	SPI_MOSI = 1;//output_high(SPI_MOSI);
}
void spi_write8(u8 address,u8 data_out,u8 sensor)
{

  DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
  //delay_us(1);
  ENABLE_PNI_Sensor(sensor);//output_low(SPI_CS);
  spi_write(address);
  spi_write(data_out);
  DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
 
}
void spi_write16(u8 address,u16 data_out,u8 sensor)
{

  DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
  //delay_us(1);
  ENABLE_PNI_Sensor(sensor);//output_low(SPI_CS);
  spi_write(address);
  spi_write(data_out>>8);
  spi_write(data_out);
  DISABLE_PNI_Sensor(sensor);// output_high(SPI_CS); 
}
u8 spi_read8(u8 address,u8 sensor)
{
   u8 read_result = 0 ;
   address |= 0x80; 
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
   delay_us(5);
   ENABLE_PNI_Sensor(sensor);//output_low(SPI_CS);
   delay_us(5);
   spi_write(address);  
   read_result = spi_read();
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
 
   return read_result ;  
}
u16 spi_read16(u8 address,u8 sensor)
{
   u16 read_result = 0 ;
   address |= 0x80;
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
   //delay_us(1);
   ENABLE_PNI_Sensor(sensor);//output_low(SPI_CS);
   //delay_us(5);
   spi_write(address);  
   read_result = spi_read();
   read_result = (read_result<<8)|spi_read() ;
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS); 
   return read_result ;  
}
u32 spi_read24(u8 address,u8 sensor)
{
   u32 read_result = 0 ;
   address |= 0x80;
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS);
   //delay_us(1);
   ENABLE_PNI_Sensor(sensor);//output_low(SPI_CS);
   //delay_us(5);
   spi_write(address);  
   read_result = spi_read();
   read_result = (read_result<<8)|spi_read() ;
   read_result = (read_result<<8)|spi_read() ;
   DISABLE_PNI_Sensor(sensor);//output_high(SPI_CS); 
	
   if((read_result>>23) == 0x01)  
   { 
//	   read_result = ~read_result;
//	   read_result&=0x7fffff;
//	   read_result += 1;
	   
	   read_result|= 0xff000000;
   } 
   return read_result ;  
}
 
 

//void spi_para_initial(void)
//{
//   pni.mode =0x79;
//   pni.timer = 0x94;
//   pni.test_flag =0x11;
//   pni.result_x_sign = 0;
//   pni.result_y_sign = 0;
//   pni.result_z_sign = 0;
//   pni.result_x = 0;
//   pni.result_y = 0;
//   pni.result_z = 0;
//   pni.cc_x = 1000;
//   pni.cc_y = 1000;
//   pni.cc_z = 1000;
//   
// //  spi_write8(TMRC,pni.timer);
//   disable_interrupts(GLOBAL);
//   spi_write8(CMM,pni.mode);
//   spi_write16(CCX,pni.cc_x);
//   spi_write16(CCY,pni.cc_y);
//   spi_write16(CCZ,pni.cc_z); 
//   enable_interrupts(GLOBAL);
//   
//   
//}


//void read_test_result()
//{
//   int16 crc_val;
//   int8 i;
//   if(input(SPI_STATE))
//   {
//      disable_interrupts(GLOBAL);
//      pni.result_x = spi_read24(MX);
//      pni.result_y = spi_read24(MY);
//      pni.result_z = spi_read24(MZ);
//      enable_interrupts(GLOBAL);
//  
//      
//      rm2100_buf[0] = (pni.result_x>>16)&0xff;
//      rm2100_buf[1] = (pni.result_x>>8)&0xff;
//      rm2100_buf[2] =  pni.result_x&0xff;
//      
//      rm2100_buf[3] = (pni.result_y>>16)&0xff;
//      rm2100_buf[4] = (pni.result_y>>8)&0xff;
//      rm2100_buf[5] =  pni.result_y&0xff;
//      
//      rm2100_buf[6] = (pni.result_z>>16)&0xff;
//      rm2100_buf[7] = (pni.result_z>>8)&0xff;
//      rm2100_buf[8] =  pni.result_z&0xff;
//      
//      crc_val = crc16(rm2100_buf, RM2100_BUF_MAX -2);
//      
//      rm2100_buf[9] = (crc_val>>8)&0xff;
//      rm2100_buf[10]=  crc_val&0xff;
//  //     /*
//      init_main_send_com();
//      send_byte(0X0D, FALSE);
//      send_byte(0X0A, FALSE);
//      for(i = 0; i < RM2100_BUF_MAX; i++)
//      {
//         send_byte(rm2100_buf[i], FALSE);//FALSE: do not need crc 
//      }
//   //  */
// /*  
//      if((pni.result_x>>23) == 0x01)
//      {
//         pni.result_x_sign = 1;
//         pni.result_x = ~pni.result_x;
//         pni.result_x&=0x7fffff;
//         pni.result_x += 1;
//      }
//      else
//         pni.result_x_sign = 0;
//      
//      if((pni.result_y>>23) == 0x01)  
//      {
//         pni.result_y_sign = 1;
//         pni.result_y = ~pni.result_y;
//         pni.result_y&=0x7fffff;
//         pni.result_y += 1;
//      }
//      else
//         pni.result_y_sign = 0;
//      
//      if((pni.result_z>>23) == 0x01)  
//      {
//         pni.result_z_sign = 1;
//         pni.result_z = ~pni.result_z;
//         pni.result_z&=0x7fffff;
//         pni.result_z += 1;
//      }
//      else
//         pni.result_z_sign = 0;
//   */
//   }
//}

//void read_para_test()
//{
//   if(pni.test_flag==1)
//   {
//      pni.test_flag = 0;
//    
//      pni.mode = spi_read8(CMM);
//      pni.timer = spi_read8(TMRC);
//      pni.cc_x = spi_read16(CCX);
//      pni.cc_y = spi_read16(CCY);
//      pni.cc_z = spi_read16(CCZ);
//     
//   }
//}

