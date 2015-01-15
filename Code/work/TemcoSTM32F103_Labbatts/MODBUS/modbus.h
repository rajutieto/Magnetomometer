#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "bitmap.h"

#define UART1_STATE	PBout(1)
#define UART2_STATE	PDout(7)

#define SERIAL_RECEIVE_TIMEOUT  	2
#define MAX_BUF    					11
#define TOTAL_UART					2
#define UART1						0
#define UART2						1

#define SEND                    	1
#define RECEIVE                 	0

#define INVALID_PACKET          	0
#define VALID_PACKET            	1


extern u16 rec_timer;
extern u8 CRChi;
extern u8 CRClo;
extern u16 CRC16_DATA; 
 
extern u8 uart_rece_count[TOTAL_UART];
extern u8 uart_dealwithTag[TOTAL_UART];
extern u8 serial_receive_timeout_count;
extern u8 Uart_Rec_Buf[TOTAL_UART][MAX_BUF];;
u8 checkData(u8 *p);
void uart_internalDeal(u8 channel); 
void uart_serial_restart(u8 channel);
 
void modbus_init(void);
u16 crc16(u8 *p, u8 length);
void init_crc16(void);
  
#endif


