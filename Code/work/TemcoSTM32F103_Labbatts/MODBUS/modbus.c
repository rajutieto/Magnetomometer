//#include "labatts.h"
//#include "timerx.h"
//#include "myiic.h"
//#include "delay.h"
#include "stdio.h"
#include "modbus.h"
#include "rm3100.h"
u8 CRChi;
u8 CRClo;
u16 CRC16_DATA; 
u8 main_rece_count;
u8 uart2_rece_count;

u8 uart_rece_count[TOTAL_UART];
u8 uart_dealwithTag[TOTAL_UART];
u8 serial_receive_timeout_count;
  
u8 Uart_Rec_Buf[TOTAL_UART][MAX_BUF];

u16 rec_timer =0;
 
void uart_serial_restart(u8 channel)
{
   uart_rece_count[channel] = 0; 
   uart_dealwithTag[channel] = 0; 
   if(channel == UART1)
	UART1_STATE  = RECEIVE; 
   else if(channel == UART2)
	   UART2_STATE  = RECEIVE;
}
 

void modbus_init(void)
{ 
   uart_serial_restart(UART1);
   uart_serial_restart(UART2);  
}
s32 Filter(u8 input_filter,s32 new_value,s32 old_value)
{      
   s32   siResult;                                 
   siResult = (old_value * input_filter + new_value) / (input_filter + 1);            
   return siResult;
   
}
void uart_internalDeal(u8 channel)
{ 
	u8 i;
	s32 temp;
	for(i=0;i<TOTAL_AXIS;i++)
	{
		temp = ((u32)Uart_Rec_Buf[channel][i*3]<<16) + ((u32)Uart_Rec_Buf[channel][i*3 + 1]<<8) + Uart_Rec_Buf[channel][i*3 + 2]; 
		if((temp>>23)==0x01) temp |= 0xff000000;
		else
			temp &= 0x7fffff;
		rm3100[channel].input[i] = Filter(rm3100[channel].filter,temp,rm3100[channel].preinput[i]);
		rm3100[channel].preinput[i] = rm3100[channel].input[i]; 
	} 	 
	
}
u16 const wCRCTalbeAbs[] =
{
0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400, 
};
void crc16_byte(u8 chChar)  
{    
    CRC16_DATA = wCRCTalbeAbs[(chChar ^ CRC16_DATA) & 15] ^ (CRC16_DATA >> 4);
    CRC16_DATA = wCRCTalbeAbs[((chChar >> 4) ^ CRC16_DATA) & 15] ^ (CRC16_DATA >> 4);        
}

u16 crc16(u8 *p, u8 length)
{
   CRC16_DATA = 0xffff ;
   while(length--)
   {
      crc16_byte(*p);
      p++;
   }
   return CRC16_DATA;
}

 
 

void init_crc16(void)
{
   CRClo = 0xff;
   CRChi = 0xff;
   CRC16_DATA = 0xffff ; 
}

u8 checkData(u8 *p)
{
   u16 crc_val; 
	
   crc_val = crc16(p, MAX_BUF -2);
   
   if(crc_val == (((u16)*(p+MAX_BUF-2) << 8) | *(p+MAX_BUF-1)))
  
      return TRUE;
   else
      return FALSE; 
}
 

