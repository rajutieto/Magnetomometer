#include <string.h>
#include "delay.h"
#include "stdio.h"
#include "modbus.h"
#include "stmflash.h" 
#include "stdlib.h" 
#include "24cxx.h"
#include "rm3100.h"
#include "RM3100_DRI.h"
#include "tcp_demo.h"
#include "led.h"
#include "rtc.h"
#include "mmc_sd.h"
#include "usart.h"	 
#define   FLASH_SECTIONS  				4			//256K / 64K
#define   FLASH_SECTION0  				0  			//  0K -  64K
#define   FALSH_SECTION1  				1  			// 64K - 128K
#define   FALSH_SECTION2  				2  			//128K - 192K
#define   FLASH_SECTION3  				3  			//192K - 256K
#define   FLASH_SECTION_SIZE    		0x10000  	//64K

#define   PROGRAM_FINISH				0x01
#define   PROGRAM_RESUME				0x1F
#define   ERASE_USED_FLASH				0x33
#define   ERASE_ALL_FLASH				0x3F
#define   PROGRAM_START					0x7F

u8 Beep_Status;
u8 Relay_Status;

u16 update_flash_status = 0; 

u16 tick_ctr;
STR_MODBUS Modbus;   

u8 uart_dealwithTag;
u8 serial_receive_timeout_count;
  
u8 Uart_Rec_Buf[MAX_BUF];
u8 Uart_Send_Buf[MAX_BUF]; 
u16 test[20];
  
static u8 rece_size;
static u16 Sendbyte_Num;
static u8 uart_rece_count;  
static u16 CRC16_DATA;  

	
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res; 
	static u8 uart_send_count = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//接收中断 
	{
		  Res = USART_ReceiveData(USART1);//(USART1->DR);		//读取接收到的数据  
		  if(uart_rece_count < MAX_BUF)
			 Uart_Rec_Buf[uart_rece_count++] = Res;
		  else
			 uart_rece_count = 0;

		  if(uart_rece_count == 7)
		  {
			   //check if it is a scan command
			   if(Uart_Rec_Buf[1] == READ_VARIABLES || Uart_Rec_Buf[1] == WRITE_VARIABLES)
			   {
					 // This starts a timer that will reset communication.  If you do not
					 // receive the full packet, it insures that the next receive will be fresh.
					 // The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
					 rece_size = DATABUFLEN;
					 serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
		       }
			   else if(((u16)Uart_Rec_Buf[2]<<8) + Uart_Rec_Buf[3] == PLUG_N_PLAY && Uart_Rec_Buf[1] == WRITE_VARIABLES)
			   {
					rece_size = DATABUFLEN_SCAN;
					serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;	
			   }
			   else if(Uart_Rec_Buf[1] == MULTIPLE_WRITE_VARIABLES)
			   {
					rece_size = Uart_Rec_Buf[6] + 9;
					serial_receive_timeout_count = rece_size/4;//from 4 to 8 to see if can resolve multiple problem

			   }
			   else
			   {
					rece_size = DATABUFLEN_DEF;
					serial_receive_timeout_count = rece_size/4;//from 4 to 8 to see if can resolve multiple problem
			   }
				
		  } 
		  // As soon as you receive the final byte, switch to SEND mode
		  else if(((uart_rece_count == rece_size ) && ((Uart_Rec_Buf[1] == MULTIPLE_WRITE_VARIABLES) ||(Uart_Rec_Buf[1] == READ_VARIABLES) || (Uart_Rec_Buf[1] == WRITE_VARIABLES)))||((uart_rece_count == 6) && (Uart_Rec_Buf[1] == CHECKONLINE)))
		  {    
			 // full packet received - turn off serial timeout
			 serial_receive_timeout_count = 0; 
			 uart_dealwithTag = VALID_PACKET; 

		  }
		  
		
     } 
	 else  if( USART_GetITStatus(USART1, USART_IT_TXE) == SET  )
     { 
		 if( uart_send_count <  Sendbyte_Num)
		 {
			 USART_SendData(USART1, Uart_Send_Buf[uart_send_count++] ); 
			 //USART_ClearFlag(USART1, USART_FLAG_TC);			 
		 }
		 else
		 { 
			 while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);//等待所有数据发送完成
			
			 USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			 uart_send_count = 0 ;  
			 uart_serial_restart();
		 }    			
     }
}  
 
void uart_serial_restart(void)
{
   uart_rece_count  = 0; 
   uart_dealwithTag  = 0;  
   UART1_STATE  = RECEIVE;  	
}
 
void init_main_send_com(void)
{ 
   UART1_STATE = SEND;
   //delay_us(100);
}

static void modbus_para_initial(void)
{ 
	AT24CXX_Read(EEP_SERIALNUMBER_LOWORD, Modbus.serial_number, 4);
	AT24CXX_Read(EEP_VERSION_NUMBER_LO, Modbus.software_rev, 2); 
	Modbus.address = AT24CXX_ReadOneByte(EEP_ADDRESS);
	Modbus.product_model = AT24CXX_ReadOneByte(EEP_PRODUCT_MODEL);
	Modbus.hardware_rev = AT24CXX_ReadOneByte(EEP_HARDWARE_REV);
	update_flash_status = AT24CXX_ReadOneByte(EEP_UPDATE_STATUS);
	Modbus.baudrate = AT24CXX_ReadOneByte(EEP_BAUD_RATE);
	if(Modbus.baudrate>1)	Modbus.baudrate = 1;
}
void modbus_init(void)
{     
	 modbus_para_initial();
	uart_serial_restart(); 
	serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
}

void init_crc16(void)
{ 
   CRC16_DATA = 0xffff ; 
} 

static u16 const wCRCTalbeAbs[] =
{
0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400, 
};

static void crc16_byte(u8 chChar)  
{    
    CRC16_DATA = wCRCTalbeAbs[(chChar ^ CRC16_DATA) & 15] ^ (CRC16_DATA >> 4);
    CRC16_DATA = wCRCTalbeAbs[((chChar >> 4) ^ CRC16_DATA) & 15] ^ (CRC16_DATA >> 4);        
}

static u16 CRC16(u8 *p, u8 length)
{
   CRC16_DATA = 0xffff ;
   while(length--)
   {
      crc16_byte(*p);
      p++;
   }
   return CRC16_DATA;
}

 
 


u8 checkData(void)
{
   u16 crc_val;
   u8 minaddr, maxaddr,i,variable_delay;
//   int16 variable_delay; 
   // given this is used in multiple places, decided to put it as an argument
 //  address = ((int16)data_buffer[2] << 8) + data_buffer[3];
   if(Uart_Rec_Buf[1] == MULTIPLE_WRITE_VARIABLES)
   {   
	   ; 
   }
   if((Uart_Rec_Buf[0] != 255) && (Uart_Rec_Buf[0] != Modbus.address) && (Uart_Rec_Buf[0] != 0))
      return FALSE;   
   if( Uart_Rec_Buf[1] == CHECKONLINE)
   {
      crc_val = CRC16(Uart_Rec_Buf,4) ; 
      if(crc_val != (u16)Uart_Rec_Buf[5]*256 + Uart_Rec_Buf[4] )
      {
         
         return FALSE;
      }
      minaddr = (Uart_Rec_Buf[2] >= Uart_Rec_Buf[3] ) ? Uart_Rec_Buf[3] : Uart_Rec_Buf[2] ;   
      maxaddr = (Uart_Rec_Buf[2] >= Uart_Rec_Buf[3] ) ? Uart_Rec_Buf[2] : Uart_Rec_Buf[3] ;   
 
      if(Modbus.address < minaddr || Modbus.address > maxaddr)
      { 
         return FALSE;
      }
      else         
      {   // in the TRUE case, we add a random delay such that the Interface can pick up the packets
			srand(tick_ctr);
			variable_delay = (rand()%40)+4;
			for ( i=0; i<variable_delay; i++)
				delay_us(500);	// equivalent to 0.5ms
			return TRUE;  
      }

   } 
   if((Uart_Rec_Buf[1] != READ_VARIABLES) && (Uart_Rec_Buf[1] != WRITE_VARIABLES)&& (Uart_Rec_Buf[1] != MULTIPLE_WRITE_VARIABLES))
      return FALSE;
 
   // plug and play feature in case there are two devices in the network with the same ID
//	if(Uart_Rec_Buf[2]*256 + Uart_Rec_Buf[3] ==  PLUG_N_PLAY)
//	{
//		if (Uart_Rec_Buf[1] == READ_VARIABLES)
//		{	// generate a random value
//			randval = rand() % 5 ; 
//			// in most cases we will return a false	
//			if(randval != RESPONSERANDVALUE)
//			{
//				plug_and_play_count++;
//				if(plug_and_play_count%5 == 0)
//				{
//					plug_and_play_count = 0;
//					randval = RESPONSERANDVALUE;
//					variable_delay = (rand()%40)+4;
//					for ( i=0; i<variable_delay; i++)
//						delay_us(50);	// delay of 0.5ms
//				}
//				else
//					return FALSE;
//			}
//			else
//			{	// in the TRUE case, we add a random delay such that the GUI can pick up the packets
//				srand(counter_heartbeat);
//				variable_delay = (rand()%40)+4;
//				for ( i=0; i<variable_delay; i++)
//					delay_us(50);	// delay of 0.5ms
//			}
//		}
//		else if (Uart_Rec_Buf[1] == WRITE_VARIABLES)
//		{	// protocol was changed around to make system more robust
//			RamRead(EEP_SERIALNUMBER_LOWORD,&serialnumber[0]);
//			RamRead(EEP_SERIALNUMBER_LOWORD+1,&serialnumber[1]);
//			RamRead(EEP_SERIALNUMBER_HIWORD,&serialnumber[2]);
//			RamRead(EEP_SERIALNUMBER_HIWORD+1,&serialnumber[3]);

//			if( (data_buffer[6]!=serialnumber[0]) || (data_buffer[7]!=serialnumber[1]) || (data_buffer[8]!=serialnumber[2]) || (data_buffer[9]!=serialnumber[3]) )
//				return FALSE;

//			srand(counter_heartbeat);
//			variable_delay = (rand()%40)+4;
//			for ( i=0; i<variable_delay; i++)
//				delay_us(50);	// delay of 0.5ms
//		}
//	}
	
   crc_val = CRC16(Uart_Rec_Buf, rece_size -2);
   
   if(crc_val == (((u16)Uart_Rec_Buf[rece_size-1] << 8) | Uart_Rec_Buf[rece_size-2]))
  
      return TRUE;
   else
      return FALSE;
}



 

void internalDeal(u16 start_address)
{    
   u16 i,temp;    
   if(Uart_Rec_Buf[1] == WRITE_VARIABLES)
   { 
		if(start_address == MODBUS_ADDRESS)
		{
			Modbus.address = Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_ADDRESS,Uart_Rec_Buf[5]);
		}  
		else if(start_address == MODBUS_UPDATE_STATUS )
		{  
			update_flash_status = Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_UPDATE_STATUS,Uart_Rec_Buf[5]);
		}
		//serial number
		//low bytes
		else if(start_address == MODBUS_SERIALNUMBER_LOWORD)
		{  
			Modbus.serial_number[1] = Uart_Rec_Buf[4];
			Modbus.serial_number[0] = Uart_Rec_Buf[5]; 	
			AT24CXX_WriteOneByte(EEP_SERIALNUMBER_LOWORD,Uart_Rec_Buf[5]);
			AT24CXX_WriteOneByte(EEP_SERIALNUMBER_LOWORD+1,Uart_Rec_Buf[4]);
		}  
		//high bytes
		else if(start_address == MODBUS_SERIALNUMBER_HIWORD)
		{ 
			Modbus.serial_number[3] = Uart_Rec_Buf[4];
			Modbus.serial_number[2] = Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_SERIALNUMBER_HIWORD,Uart_Rec_Buf[5]);
			AT24CXX_WriteOneByte(EEP_SERIALNUMBER_HIWORD+1,Uart_Rec_Buf[4]);
		}
		//firmware version
		else if(start_address == MODBUS_VERSION_NUMBER_LO)
		{
			Modbus.software_rev[1] = Uart_Rec_Buf[4];
			Modbus.software_rev[0] = Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_VERSION_NUMBER_LO,Uart_Rec_Buf[5]);
			AT24CXX_WriteOneByte(EEP_VERSION_NUMBER_LO+1,Uart_Rec_Buf[4]);
		}  	 
		//product model
		else if(start_address == MODBUS_PRODUCT_MODEL)
		{ 
			Modbus.product_model = Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_PRODUCT_MODEL,Uart_Rec_Buf[5]);
		}
		//product hardware
		else if(start_address == MODBUS_HARDWARE_REV)
		{ 
			Modbus.hardware_rev = Uart_Rec_Buf[5];
			AT24CXX_WriteOneByte(EEP_HARDWARE_REV,Uart_Rec_Buf[5]);
		} 
		//pni sensor0 parameter
		else if(start_address == MODBUS_PNI0_MODE)
		{ 
			pni[0].Mode = Uart_Rec_Buf[5],
			AT24CXX_WriteOneByte(EEP_PNI0_MODE,Uart_Rec_Buf[5]); 
			spi_write8(CMM,pni[0].Mode,0); 
		}
		else if(start_address== MODBUS_PNI0_UPDATE_RATE)
		{ 
			pni[0].Update_Rate = Uart_Rec_Buf[5];
			AT24CXX_WriteOneByte(EEP_PNI0_UPDATE_RATE,Uart_Rec_Buf[5]); 
			spi_write8(TMRC,pni[0].Update_Rate,0); 
		}
		else if((start_address >=MODBUS_PNI0_GAIN_X)&&(start_address <= MODBUS_PNI0_GAIN_Z))
		{
			temp = start_address - MODBUS_PNI0_GAIN_X;
			pni[0].Gian[temp] = ((u16)Uart_Rec_Buf[4]<<8)+Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_PNI0_GAIN_X + temp*2,Uart_Rec_Buf[5]);
			AT24CXX_WriteOneByte(EEP_PNI0_GAIN_X + temp*2 + 1,Uart_Rec_Buf[4]); 
			spi_write16(CCX,pni[0].Gian[temp],0); 
		}
		else if((start_address >=MODBUS_PNI0_FILTER_X)&&(start_address <= MODBUS_PNI0_FILTER_Z))
		{ 
			temp = start_address - MODBUS_PNI0_FILTER_X;
			pni[0].Filter[temp]=Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_PNI0_FILTER_X + temp,Uart_Rec_Buf[5]);
		} 
		//pni sensor1 parameter
		else if(start_address == MODBUS_PNI1_MODE)
		{ 
			pni[1].Mode = Uart_Rec_Buf[5],
			AT24CXX_WriteOneByte(EEP_PNI1_MODE,Uart_Rec_Buf[5]);
			spi_write8(CMM,pni[1].Mode,1); 
		}
		else if(start_address== MODBUS_PNI1_UPDATE_RATE)
		{ 
			pni[1].Update_Rate = Uart_Rec_Buf[5];
			AT24CXX_WriteOneByte(EEP_PNI1_UPDATE_RATE,Uart_Rec_Buf[5]);
			spi_write8(TMRC,pni[1].Update_Rate,1); 
		}
		else if((start_address >=MODBUS_PNI1_GAIN_X)&&(start_address <= MODBUS_PNI1_GAIN_Z))
		{
			temp = start_address - MODBUS_PNI1_GAIN_X;
			pni[1].Gian[temp] = ((u16)Uart_Rec_Buf[4]<<8)+Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_PNI1_GAIN_X + temp*2,Uart_Rec_Buf[5]);
			AT24CXX_WriteOneByte(EEP_PNI1_GAIN_X + temp*2 + 1,Uart_Rec_Buf[4]);
			spi_write16(CCX,pni[1].Gian[temp],1); 
		}
		else if((start_address >=MODBUS_PNI1_FILTER_X)&&(start_address <= MODBUS_PNI1_FILTER_Z))
		{ 
			temp = start_address - MODBUS_PNI1_FILTER_X;
			pni[1].Filter[temp]=Uart_Rec_Buf[5]; 
			AT24CXX_WriteOneByte(EEP_PNI1_FILTER_X + temp,Uart_Rec_Buf[5]);
		} 	  
		//pni sensor parameter set
		else if(start_address == MODBUS_RESET_PNI_INPUT)
		{
			pni_para.reset_input_flag = Uart_Rec_Buf[5]; 
		}
		else if(start_address== MODBUS_RECORD_NUMBER_OPTO_OFF)
		{
			pni_para.Record_Number_Opto_Off = Uart_Rec_Buf[5];
			AT24CXX_WriteOneByte(EEP_RECORD_NUMBER_OPTO_OFF,Uart_Rec_Buf[5]);
		}
		else if(start_address == MODBUS_READ_PNI_CONFIG)
		{  
			pni_para.read_config_flag= Uart_Rec_Buf[5];
		}
		//EARTH NET
		else if(start_address == MODBUS_IP_MODE)
		{  
			if(Uart_Rec_Buf[5] < 2)
			{
				ip_mode_ghost = Uart_Rec_Buf[5];
				if(ip_mode_ghost == 0)
				{
					AT24CXX_Read(EEP_MAC_ADDRESS,mac_address,6); 
					AT24CXX_Read(EEP_IP_ADDRESS ,ip_address_ghost,4); 
					AT24CXX_Read(EEP_SUBNET_MASK_ADDRESS,subnet_mask_address_ghost,4); 
					AT24CXX_Read(EEP_GATEWAY_ADDRESS,gateway_address_ghost,4);
					AT24CXX_Read(EEP_TCP_PORT,listen_port_at_tcp_server_mode_ghost,2);  
				}
				enable_ghost = ENABLE;
			}
		}
		else if((start_address >=MODBUS_MAC_ADDRESS_0)&&(start_address <= MODBUS_MAC_ADDRESS_5))
		{
			temp = start_address - MODBUS_MAC_ADDRESS_0;
			mac_address[temp]= Uart_Rec_Buf[5];
			AT24CXX_WriteOneByte(EEP_MAC_ADDRESS + temp,Uart_Rec_Buf[5]);
		}
		else if((start_address >=MODBUS_IP_ADDRESS_0)&&(start_address <= MODBUS_IP_ADDRESS_3))
		{
			temp = start_address - MODBUS_IP_ADDRESS_0;
			ip_address_ghost[temp] = Uart_Rec_Buf[5]; 
//			AT24CXX_WriteOneByte(EEP_IP_ADDRESS + temp,Uart_Rec_Buf[5]);
		}
		else if((start_address >=MODBUS_SUBNET_MASK_ADDRESS_0)&&(start_address <= MODBUS_SUBNET_MASK_ADDRESS_3))
		{
			temp = start_address - MODBUS_SUBNET_MASK_ADDRESS_0;
			subnet_mask_address_ghost[temp] = Uart_Rec_Buf[5]; 
//			AT24CXX_WriteOneByte(EEP_SUBNET_MASK_ADDRESS + temp,Uart_Rec_Buf[5]);
		}
		else if((start_address >=MODBUS_GATEWAY_ADDRESS_0)&&(start_address <= MODBUS_GATEWAY_ADDRESS_3))
		{
			temp = start_address - MODBUS_GATEWAY_ADDRESS_0;
			gateway_address_ghost[temp] = Uart_Rec_Buf[5]; 
//			AT24CXX_WriteOneByte(EEP_GATEWAY_ADDRESS + temp,Uart_Rec_Buf[5]);
		}
		else if(start_address == MODBUS_TCP_PORT)
		{
			listen_port_at_tcp_server_mode_ghost[1] = Uart_Rec_Buf[4];
			listen_port_at_tcp_server_mode_ghost[0] = Uart_Rec_Buf[5];
//			AT24CXX_WriteOneByte(EEP_TCP_PORT + 1,Uart_Rec_Buf[4]);
//			AT24CXX_WriteOneByte(EEP_TCP_PORT,Uart_Rec_Buf[5]);
		}
		else if(start_address == MODBUS_TCP_SERVER_ENABLE)
		{ 
			enable_ghost = Uart_Rec_Buf[5];
		}
		else if(start_address == MODBUS_BEEP_STATUS)
		{ 
			Beep_Status = Uart_Rec_Buf[5];
			if(Beep_Status == 1) BEEP_CONTROL = 1;
			else if(Beep_Status ==0) BEEP_CONTROL = 0;
		}
		else if(start_address == MODBUS_RELAY_STATUS)
		{ 
			Relay_Status = Uart_Rec_Buf[5];
			if(Relay_Status == 1) RELAY_CONTROL = 1;
			else if(Relay_Status ==0) RELAY_CONTROL = 0;
		}
		else if(start_address == MODBUS_RTC_HOUR)
			{
//				if(Uart_Rec_Buf[5]<24)
//				{
					
					memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
					calendar_ghost.hour = Uart_Rec_Buf[5]; 
//					Time_Adjust();
 					calendar_ghost_enable = ENABLE;
//				}
			}
			else if(start_address == MODBUS_RTC_MIN)
			{
//				if(Uart_Rec_Buf[5] < 60)
//				{
					memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
					calendar_ghost.min = Uart_Rec_Buf[5];
//					Time_Adjust();
 					calendar_ghost_enable = ENABLE;
//				}
			} 
			else if(start_address == MODBUS_RTC_SEC)
			{
//				if(Uart_Rec_Buf[5] < 60)
//				{
					memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
					calendar_ghost.sec = Uart_Rec_Buf[5]; 
//					Time_Adjust();
					calendar_ghost_enable = ENABLE;
//				}
			} 
			else if(start_address == MODBUS_RTC_YEAR)
			{ 
				memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
				calendar_ghost.w_year = ((u8)Uart_Rec_Buf[4] <<8) | Uart_Rec_Buf[5];
//				Time_Adjust();
				calendar_ghost_enable = ENABLE;
			} 
			else if(start_address == MODBUS_RTC_MONTH)
			{
//				if((Uart_Rec_Buf[5] <= 12)&&(Uart_Rec_Buf[5] >=1))
//				{
					memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
					calendar_ghost.w_month = Uart_Rec_Buf[5];
//					Time_Adjust();
					calendar_ghost_enable = ENABLE;
//				}
			} 
			else if(start_address == MODBUS_RTC_DATE)
			{
//				if((Uart_Rec_Buf[5] <= 31)&&(Uart_Rec_Buf[5] >=1))
//				{
					memcpy((void *)&calendar_ghost,(void *)&calendar,sizeof(_calendar_obj));
					calendar_ghost.w_date = Uart_Rec_Buf[5]; 
//					Time_Adjust();
					calendar_ghost_enable = ENABLE;
//				}
			}
			else if(start_address == MODBUS_RTC_GHOST_ENABLE)
			{
				calendar_ghost_enable = Uart_Rec_Buf[5];
			}	
			else if(start_address == MODBUS_BAUD_RATE)
			{
				if(( Uart_Rec_Buf[5] == 0)|| (Uart_Rec_Buf[5] == 1))
				{
					Modbus.baudrate	= Uart_Rec_Buf[5];
					if(Modbus.baudrate)
						uart1_init(19200);
					else
						uart1_init(9600); 
					AT24CXX_WriteOneByte(EEP_BAUD_RATE, Modbus.baudrate);
				}
			}	
		 
			
   }
   else if(Uart_Rec_Buf[1] == MULTIPLE_WRITE_VARIABLES)
   { 
		;
   }
   else if(Uart_Rec_Buf[1] == READ_VARIABLES)
   {
		; 
   }
   // --------------- update initialize --------------------
	if (update_flash_status == PROGRAM_START)//0X7F
	{
		 SoftReset();	
	}
	   	
}

void send_byte(u8 buffer, u8 crc)
{ 
   
   Uart_Send_Buf[Sendbyte_Num]  = buffer; 
   if(Sendbyte_Num < MAX_BUF) Sendbyte_Num++;
   if(crc == TRUE)
   {
      crc16_byte(buffer);
   }   
}

#define READY   0
#define DONE    1
static void USART_SendDataString( u8 status)
{ 
	if(status == READY)
		Sendbyte_Num = 0;
	else if(status == DONE)
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//start to transmit
}
//the routine organize  data back and send the data to buffer.
void responseData(u16 address)
{
    
   u8 num;
   u8 i; 
   u8 temp;
   
   USART_SendDataString(READY);     //INITIAL SEND BUFFER
	
   if(Uart_Rec_Buf[1] == WRITE_VARIABLES)
   {
      for(i = 0; i < rece_size; i++)
      { 
		  send_byte(Uart_Rec_Buf[i], FALSE);//FALSE: do not need crc 
      } 
   } 
   else if(Uart_Rec_Buf[1] == READ_VARIABLES)
   {
		num = Uart_Rec_Buf[5];      
		send_byte(Uart_Rec_Buf[0], TRUE);
		send_byte(Uart_Rec_Buf[1], TRUE);
//	    if( address == EEP_ADDRESS_PLUG_N_PLAY)
//			send_byte((num << 1)+4, TRUE);
//		else
			send_byte(num << 1, TRUE);
	  
		for(i = 0; i < num; i++)
		{ 
            //serial number
			//low bytes
			if(((i + address) >= MODBUS_SERIALNUMBER_LOWORD)&&((i + address) <= MODBUS_SERIALNUMBER_HIWORD + 1))
			{
				send_byte(0, TRUE);
				send_byte(Modbus.serial_number[(i + address)- MODBUS_SERIALNUMBER_LOWORD], TRUE); 
			}  
			//the version of firmware
			else if((i + address) == MODBUS_VERSION_NUMBER_LO)
			{
				send_byte(0, TRUE);
				send_byte(Modbus.software_rev[0], TRUE);
			}
			else if((i + address) == MODBUS_VERSION_NUMBER_HI)
			{
				send_byte(0, TRUE);
				send_byte(Modbus.software_rev[1], TRUE);
			}
			//modbus address
			else if((i + address) == MODBUS_ADDRESS)
			{
				send_byte(0, TRUE);
				send_byte(Modbus.address, TRUE); 
			}
			//product model
			else if((i + address) == MODBUS_PRODUCT_MODEL)
			{
				send_byte(0, TRUE); 
				send_byte(Modbus.product_model, TRUE);
			}
			//product hardware
			else if((i + address) == MODBUS_HARDWARE_REV)
			{
				send_byte(0, TRUE);
				send_byte(Modbus.hardware_rev, TRUE);
			}
			else if((i + address) == MODBUS_UPDATE_STATUS)
			{
				send_byte(0, TRUE);
				send_byte(update_flash_status, TRUE); 
			}
			//pni sensor0 parameter
			else if((i + address) == MODBUS_PNI0_MODE)
			{
				send_byte(0, TRUE);
				send_byte(pni[0].Mode, TRUE);
			}
			else if((i + address) == MODBUS_PNI0_UPDATE_RATE)
			{
				send_byte(0, TRUE);
				send_byte(pni[0].Update_Rate, TRUE);
			}
			else if(((i + address) >=MODBUS_PNI0_GAIN_X)&&((i + address) <= MODBUS_PNI0_GAIN_Z))
			{
				send_byte(pni[0].Gian[i + address - MODBUS_PNI0_GAIN_X]>>8, TRUE);
				send_byte(pni[0].Gian[i + address - MODBUS_PNI0_GAIN_X], TRUE); 
			}
			else if(((i + address) >=MODBUS_PNI0_FILTER_X)&&((i + address) <= MODBUS_PNI0_FILTER_Z))
			{
				send_byte(0, TRUE);
				send_byte(pni[0].Filter[i + address - MODBUS_PNI0_FILTER_X], TRUE); 
			} 
			else if(((i + address) >=MODBUS_PNI0_INPUT_L_X)&&((i + address) <= MODBUS_PNI0_INPUT_H_Z))
			{
				temp = i + address - MODBUS_PNI0_INPUT_L_X;
				if(temp%2 == 0)
				{
					send_byte(pni[0].PreInput[temp/2]>>8, TRUE);
					send_byte(pni[0].PreInput[temp/2], TRUE); 
				}
				else
				{
					send_byte(pni[0].PreInput[temp/2]>>24, TRUE);
					send_byte(pni[0].PreInput[temp/2]>>16, TRUE); 
				}
			} 
			//pni sensor1 parameter
			else if((i + address) == MODBUS_PNI1_MODE)
			{
				send_byte(0, TRUE);
				send_byte(pni[1].Mode, TRUE);
			}
			else if((i + address) == MODBUS_PNI1_UPDATE_RATE)
			{
				send_byte(0, TRUE);
				send_byte(pni[1].Update_Rate, TRUE);
			}
			else if(((i + address) >=MODBUS_PNI1_GAIN_X)&&((i + address) <= MODBUS_PNI1_GAIN_Z))
			{
				send_byte(pni[1].Gian[i + address - MODBUS_PNI1_GAIN_X]>>8, TRUE);
				send_byte(pni[1].Gian[i + address - MODBUS_PNI1_GAIN_X], TRUE); 
			}
			else if(((i + address) >=MODBUS_PNI1_FILTER_X)&&((i + address) <= MODBUS_PNI1_FILTER_Z))
			{
				send_byte(0, TRUE);
				send_byte(pni[1].Filter[i + address - MODBUS_PNI1_FILTER_X], TRUE); 
			} 
			else if(((i + address) >=MODBUS_PNI1_INPUT_L_X)&&((i + address) <= MODBUS_PNI1_INPUT_H_Z))
			{
				temp = i + address - MODBUS_PNI1_INPUT_L_X;
				if(temp%2 == 0)
				{
					send_byte(pni[1].PreInput[temp/2]>>8, TRUE);
					send_byte(pni[1].PreInput[temp/2], TRUE); 
				}
				else
				{
					send_byte(pni[1].PreInput[temp/2]>>24, TRUE);
					send_byte(pni[1].PreInput[temp/2]>>16, TRUE); 
				}
			} 
			//pni sensor parameter set
			else if((i + address) == MODBUS_RESET_PNI_INPUT)
			{
				send_byte(0, TRUE);
				send_byte(pni_para.reset_input_flag, TRUE);
			}
			else if((i + address) == MODBUS_RECORD_NUMBER_OPTO_OFF)
			{
				send_byte(pni_para.Record_Number_Opto_Off>>8, TRUE);
				send_byte(pni_para.Record_Number_Opto_Off, TRUE);
			}
			else if((i + address) == MODBUS_OPTO_STATUS)
			{
				send_byte(0, TRUE);
				send_byte(pni_para.opto_status, TRUE);
			}
			else if((i + address) == MODBUS_OPTO_TRIGGLE_NUMBER)
			{
				send_byte(pni_para.opto_triggle_number>>8, TRUE);
				send_byte(pni_para.opto_triggle_number, TRUE);
			}
			else if((i + address) == MODBUS_PNI_RECORD_NUMBER)
			{
				send_byte(pni_para.pni_record_number>>8, TRUE);
				send_byte(pni_para.pni_record_number, TRUE);
			}
			else if((i + address) == MODBUS_READ_PNI_CONFIG)
			{
				send_byte(0, TRUE);
				send_byte(pni_para.read_config_flag, TRUE);
			}
			//EARTH NET
			else if((i + address) == MODBUS_IP_MODE)
			{
				send_byte(0, TRUE);
				send_byte(ip_mode_ghost, TRUE);
			}
			else if(((i + address) >=MODBUS_MAC_ADDRESS_0)&&((i + address) <= MODBUS_MAC_ADDRESS_5))
			{
				send_byte(0, TRUE);
				send_byte(mac_address[i + address - MODBUS_MAC_ADDRESS_0], TRUE); 
			}
			else if(((i + address) >=MODBUS_IP_ADDRESS_0)&&((i + address) <= MODBUS_IP_ADDRESS_3))
			{
				send_byte(0, TRUE);
				send_byte(ip_address_ghost[i + address - MODBUS_IP_ADDRESS_0], TRUE); 
			}
			else if(((i + address) >=MODBUS_SUBNET_MASK_ADDRESS_0)&&((i + address) <= MODBUS_SUBNET_MASK_ADDRESS_3))
			{
				send_byte(0, TRUE);
				send_byte(subnet_mask_address_ghost[i + address - MODBUS_SUBNET_MASK_ADDRESS_0], TRUE); 
			}
			else if(((i + address) >=MODBUS_GATEWAY_ADDRESS_0)&&((i + address) <= MODBUS_GATEWAY_ADDRESS_3))
			{
				send_byte(0, TRUE);
				send_byte(gateway_address_ghost[i + address - MODBUS_GATEWAY_ADDRESS_0], TRUE); 
			}
			else if((i + address) == MODBUS_TCP_PORT)
			{
				send_byte(listen_port_at_tcp_server_mode_ghost[1], TRUE);
				send_byte(listen_port_at_tcp_server_mode_ghost[0], TRUE);
			}
			else if((i + address) == MODBUS_TCP_SERVER_ENABLE)
			{
				send_byte(0, TRUE);
				send_byte(enable_ghost, TRUE);
			}
			else if((i + address) == MODBUS_BEEP_STATUS)
			{
				send_byte(0, TRUE);
				send_byte(Beep_Status, TRUE);
			}
			else if((i + address) == MODBUS_RELAY_STATUS)
			{
				send_byte(0, TRUE);
				send_byte(Relay_Status, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_HOUR)
			{
				send_byte(0, TRUE);
				send_byte(calendar.hour, TRUE);
			}
			else if((i + address) == MODBUS_RTC_MIN)
			{
				send_byte(0, TRUE);
				send_byte(calendar.min, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_SEC)
			{
				send_byte(0, TRUE);
				send_byte(calendar.sec, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_YEAR)
			{
				send_byte(calendar.w_year>>8, TRUE);
				send_byte(calendar.w_year, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_MONTH)
			{
				send_byte(0, TRUE);
				send_byte(calendar.w_month, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_DATE)
			{
				send_byte(0, TRUE);
				send_byte(calendar.w_date, TRUE);
			} 
			else if((i + address) == MODBUS_RTC_WEEK)
			{
				send_byte(0, TRUE);
				send_byte(calendar.week, TRUE);
			}
			else if((i + address) == MODBUS_RTC_GHOST_ENABLE)
			{
				send_byte(0, TRUE);
				send_byte(calendar_ghost_enable, TRUE);
			}
			else if((i + address) == MODBUS_SD_CARD_STATUS)
			{
				send_byte(0, TRUE);
				send_byte(SD_Card_Status, TRUE);
			}
			else if((i + address) == MODBUS_BAUD_RATE)
			{
				send_byte(0, TRUE);
				send_byte(Modbus.baudrate, TRUE);
			}
 
			//ONLY FOR TEST 
			else if(((i + address) >=MODBUS_TEST)&&((i + address) < (MODBUS_TEST+20)))
			{
				send_byte(test[i + address - MODBUS_TEST]>>8, TRUE);
				send_byte(test[i + address - MODBUS_TEST], TRUE); 
			}
			  
			else
			{  
				send_byte(0, TRUE);
				send_byte(0, TRUE); 
			}
		}//end of number

		send_byte(CRC16_DATA, FALSE);// crclo
		send_byte(CRC16_DATA>>8, FALSE);// crchi 
   }
	else if(Uart_Rec_Buf[1] == MULTIPLE_WRITE_VARIABLES)
	{
		// --- response to a multiple write function ---
		// the 6 first bits are the same and then send the crc bits
		for ( i=0; i<6; i++)
		{	
			send_byte(i/*Uart_Rec_Buf[i]*/,TRUE); 
		}	
		// send the two last CRC bits
		send_byte(CRC16_DATA, FALSE);// crclo
		send_byte(CRC16_DATA>>8, FALSE);// crchi  
	}   
    else if(Uart_Rec_Buf[1] == CHECKONLINE)
    {
		 
		send_byte(Uart_Rec_Buf[0],TRUE);  
		// send second byte of information 	
		send_byte(Uart_Rec_Buf[1],TRUE);  
		// send address of device 
		send_byte(Uart_Rec_Buf[2],TRUE);  
		// send Serial1 
		send_byte(Modbus.serial_number[0],TRUE);  
		// send Serial2 
		send_byte(Modbus.serial_number[1],TRUE); 
		// send Serial3 
		send_byte(Modbus.serial_number[2],TRUE);  
		// send Serial4 
		send_byte(Modbus.serial_number[3],TRUE);  
		// send the two last CRC bits
		send_byte(CRC16_DATA, FALSE);// crclo
		send_byte(CRC16_DATA>>8, FALSE);// crchi           
   }
   USART_SendDataString(DONE);   //START TO TRANSMIT   
}

void SoftReset(void)
{
	__set_FAULTMASK(1);      // 关闭所有中断
	NVIC_SystemReset();      // 复位
}
