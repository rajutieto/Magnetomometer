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

typedef struct
{	//STORE IN EEPROM	
	U8_T serial_number[4];			//4
	U8_T software_rev[2];			//2
	U8_T address;					//1
	U8_T product_model;				//1
	U8_T hardware_rev;				//1
	U8_T init_wr_time;				//1
	U8_T address_plug_n_play;		//1
	U8_T time_zone;					//1
	U8_T baudrate;					//1
	U8_T daylight_enable;			//1
	U8_T daylight_status;			//1
	U8_T reset_flash;				//1
	U8_T update_status;				//1
	U8_T refresh_status;			//1
	U8_T refresh_flash;				//1
	U16_T refresh_flash_freq;		//1
	U8_T sn_write_flag;				//1

	//STORE IN RAM
	union
	{
		U8_T all[8];
		struct 
		{
			U8_T sec;
			U8_T min;
			U8_T hour;
			U8_T day;
			U8_T week;
			U8_T mon;
			U8_T year;
			U8_T century;	
		}Clk;
	}Time;							//8
} STR_MODBUS;

extern STR_MODBUS Modbus;


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


