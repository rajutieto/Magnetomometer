#include "RM3100_DRI.h"
#include "rm3100.h"  
#include "delay.h" 
#include "24cxx.h"
#include "modbus.h"
#include "led.h"


 

STR_PNI_BUF pni_buffer[MAX_RECORD_BUFFER+1];
STR_PNI_BUF TCP_Buffer[MAX_RECORD_BUFFER+1];
STR_MAGNETIC pni[2];

#define Opto_Capture              PDin(4)
#define Opto_Capture_OUT              PDout(4)

STR_PNI_PARA   pni_para;

static void read_pni_config(void)
{  
	u8 i;
	for(i=0;i<2;i++)
	{
		pni[i].Mode = spi_read8(CMM,i);
		pni[i].Update_Rate  = spi_read8(TMRC,i);
		pni[i].Gian[X_AXIS] = spi_read16(CCX,i);
		pni[i].Gian[Y_AXIS] = spi_read16(CCY,i);
		pni[i].Gian[Z_AXIS] = spi_read16(CCZ,i); 
	}
//	pni[0].Update_Rate = spi_read8(TMRC,0);
//	pni[1].Update_Rate = spi_read8(TMRC,1);
}
static void set_pni_config(void)
{  
	u8 i;
	for(i=0;i<2;i++)
	{
		spi_write8(CMM,pni[i].Mode,i);
		spi_write8(TMRC,pni[i].Update_Rate,i);
		spi_write16(CCX,pni[i].Gian[X_AXIS],i);
		spi_write16(CCY,pni[i].Gian[Y_AXIS],i);
		spi_write16(CCZ,pni[i].Gian[Z_AXIS],i); 
	}  
}  
static void Rm3100_Initial(void)
{
	u8 i,j;
	PNI_SPI_Init();
    pni_para.Record_Number_Opto_Off = 0;
    pni_para.record_over_flag = FALSE; 
    pni_para.opto_triggle_number = 0;
	pni_para.opto_status = OFF;
	pni_para.reset_input_flag = FALSE;
	for(i=0;i<2;i++)
	{
		for(j=0;j<TOTAL_AXIS;j++)
		{     
			pni[i].OffSet[j] = 0;
			pni[i].PreInput[j] = 0;
			pni[i].InPut[j]	= 0;		
		} 
	} 
	
	pni[0].Mode =  AT24CXX_ReadOneByte(EEP_PNI0_MODE); 
	pni[0].Update_Rate =  AT24CXX_ReadOneByte(EEP_PNI0_UPDATE_RATE ); 
	
	for(i=0;i<3;i++)
	{
		pni[0].Gian[i] = ((u16)AT24CXX_ReadOneByte(EEP_PNI0_GAIN_X + i*2 + 1)<<8) \
		+ AT24CXX_ReadOneByte(EEP_PNI0_GAIN_X + i*2);
	}
	for(i=0;i<3;i++)	  
	{
		pni[0].Filter[i]= AT24CXX_ReadOneByte(EEP_PNI0_FILTER_X + i);
	}

	pni[1].Mode =  AT24CXX_ReadOneByte(EEP_PNI1_MODE); 
	pni[1].Update_Rate =  AT24CXX_ReadOneByte(EEP_PNI1_UPDATE_RATE ); 
	
	for(i=0;i<3;i++)
	{
		pni[1].Gian[i] = ((u16)AT24CXX_ReadOneByte(EEP_PNI1_GAIN_X + i*2 + 1)<<8) \
		+ AT24CXX_ReadOneByte(EEP_PNI1_GAIN_X + i*2);
	}
	for(i=0;i<3;i++)	  
	{
		pni[1].Filter[i]= AT24CXX_ReadOneByte(EEP_PNI1_FILTER_X + i);
	}
	
	pni_para.Record_Number_Opto_Off =  AT24CXX_ReadOneByte(EEP_RECORD_NUMBER_OPTO_OFF); 
	set_pni_config();
	pni_para.read_config_flag = TRUE;
}
static void Opto_Initial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	
	 
}
static s32 Filter(u8 input_filter,s32 new_value,s32 old_value)
{      
   s32   siResult;                                 
   siResult = (old_value * input_filter + new_value) / (input_filter + 1);            
   return siResult;
   
}
static void vPNI_RecordTask( void *pvParameters )
{ 
	u16 i,j,temp; 
	static u16 Record_Over=0;
	static u16 pni_record_counter=0;
//	static u16 run_timer;
	s32 pni_temp[3];
	
	Rm3100_Initial();
	Opto_Initial();
	
	delay_ms(100);
	
    for( ;; )
	{     
//		run_timer++;
//		if(run_timer%100 == 0) LED3 = ~LED3; 
		
		// read pni sensor
		if(SPI_STATE0 == 1)
		{
			pni_temp[X_AXIS]  =spi_read24(MX,PNI_SENSOR0);
			pni_temp[Y_AXIS]  =spi_read24(MY,PNI_SENSOR0);
			pni_temp[Z_AXIS]  =spi_read24(MZ,PNI_SENSOR0);
			for(i=0;i<TOTAL_AXIS;i++)
				pni[PNI_SENSOR0].InPut[i] = Filter(pni[PNI_SENSOR0].Filter[i],pni_temp[i],pni[PNI_SENSOR0].InPut[i]);			
			
			test[2]++;
		}
		if(SPI_STATE1 == 1)
		{
			pni_temp[X_AXIS]  =spi_read24(MX,PNI_SENSOR1);
			pni_temp[Y_AXIS]  =spi_read24(MY,PNI_SENSOR1);
			pni_temp[Z_AXIS]  =spi_read24(MZ,PNI_SENSOR1);
			for(i=0;i<TOTAL_AXIS;i++)
				pni[PNI_SENSOR1].InPut[i] = Filter(pni[PNI_SENSOR1].Filter[i],pni_temp[i],pni[PNI_SENSOR1].InPut[i]);			
			
			test[3]++;
		}
		//read pni sensor config
		if(pni_para.read_config_flag == TRUE)
		{ 
			pni_para.read_config_flag = FALSE;
			read_pni_config();
		}
 		//RESET INPUT;
		if(pni_para.reset_input_flag == TRUE)
		{ 
			pni_para.reset_input_flag = FALSE;
			for(i=0;i<2;i++)
			{
				for(j=0;j<TOTAL_AXIS;j++)
				{     
					pni[i].OffSet[j] = pni[i].InPut[j];	
				} 
			}
		}
		//set offset 
		for(i=0;i<2;i++)
		{
			for(j=0;j<TOTAL_AXIS;j++)
			{     
				pni[i].PreInput[j] = pni[i].InPut[j] - pni[i].OffSet[j];	
			} 
		} 
          	
		//print buffer	
		if(pni_para.record_over_flag == FALSE)
		{ 
				 
				for(i=0;i<TOTAL_AXIS;i++)		//record pni value to a big buffer
				{    
					pni_buffer[pni_record_counter].Sensor0[i] = pni[PNI_SENSOR0].PreInput[i];
					pni_buffer[pni_record_counter].Sensor1[i] = pni[PNI_SENSOR1].PreInput[i];
				}	 
			 
				if(Opto_Capture == 1)   //opto off
				{
					pni_para.opto_status = OFF; 
					pni_buffer[pni_record_counter].Opto_Status  = OFF;
					if (pni_record_counter == pni_para.Record_Number_Opto_Off)
					{
						for(j=0;j < pni_record_counter;j++)//<<1
						{
							temp =j + 1; 
							for(i=0;i<TOTAL_AXIS;i++)
							{    
								pni_buffer[j].Sensor0[i] = pni_buffer[temp].Sensor0[i];
								pni_buffer[j].Sensor1[i] = pni_buffer[temp].Sensor1[i];
							}	 
			 
						}
					}
					else if(pni_record_counter<pni_para.Record_Number_Opto_Off) 
						pni_record_counter++;
					else if((pni_record_counter>pni_para.Record_Number_Opto_Off)&&(pni_record_counter<Record_Over))
						pni_record_counter++;
					else
					{
						pni_para.record_over_flag = TRUE; 
						pni_para.pni_record_number = pni_record_counter;
						pni_para.opto_triggle_number++;
					}
				}
				else   //opto is on
				{
					pni_para.opto_status = ON; 
					pni_buffer[pni_record_counter].Opto_Status  = ON;
					if(pni_record_counter<MAX_RECORD_BUFFER)
							pni_record_counter++; 
					Record_Over = pni_record_counter+pni_para.Record_Number_Opto_Off;
					if(Record_Over >MAX_RECORD_BUFFER) Record_Over = MAX_RECORD_BUFFER;	
				}
		}
		else
				pni_record_counter = 0;
		delay_ms(5);
    }
}
void EXTI3_IRQHandler(void)
{
	if(EXTI->PR & (1 << 3))	//是3线的中断
	{    
		test[0]++;
//		pni[PNI_SENSOR0].InPut[X_AXIS]  =spi_read24(MX,PNI_SENSOR0);
//		pni[PNI_SENSOR0].InPut[Y_AXIS]  =spi_read24(MY,PNI_SENSOR0);
//		pni[PNI_SENSOR0].InPut[Z_AXIS]  =spi_read24(MZ,PNI_SENSOR0);
		EXTI->PR = (1 << 3);	//清除LINE3上的中断标志位
	}
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & (1 << 5))	//是5线的中断
	{    
		test[1]++;
//		pni[PNI_SENSOR1].InPut[X_AXIS]  =spi_read24(MX,PNI_SENSOR1);
//		pni[PNI_SENSOR1].InPut[Y_AXIS]  =spi_read24(MY,PNI_SENSOR1);
//		pni[PNI_SENSOR1].InPut[Z_AXIS]  =spi_read24(MZ,PNI_SENSOR1);
		EXTI->PR  = (1 << 5);	//清除LINE8上的中断标志位
	}
	 
}
 
void vStartPNITask( portBASE_TYPE uxPriority)
{ 
	xTaskCreate( vPNI_RecordTask, ( signed portCHAR * ) "PNITask", configMINIMAL_STACK_SIZE, NULL, uxPriority , NULL );	 	
}
  
