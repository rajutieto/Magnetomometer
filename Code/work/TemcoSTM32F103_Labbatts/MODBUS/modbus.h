#ifndef _MODBUS_H_
#define _MODBUS_H_
#include "bitmap.h"
#define UART1_STATE						PAout(8)  

#define UART1							0
#define UART2							1 

#define SEND                    		1
#define RECEIVE                 		0

#define INVALID_PACKET          		0
#define VALID_PACKET            		1

#define READ_VARIABLES              	0x03
#define WRITE_VARIABLES             	0x06
#define MULTIPLE_WRITE_VARIABLES    	0x10
#define CHECKONLINE                 	0x19 
#define PLUG_N_PLAY				   		0x0a 

#define MAX_BUF    					   	255   //the buffer of send

#define DATABUFLEN                     	8
#define DATABUFLEN_SCAN                	12
#define SENDPOOLLEN                    	8
#define DATABUFLEN_DEF                 	150

#define SERIAL_RECEIVE_TIMEOUT        	2
#define RESPONSERANDVALUE	           	1

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
//	union
//	{
//		U8_T all[8];
//		struct 
//		{
//			U8_T sec;
//			U8_T min;
//			U8_T hour;
//			U8_T day;
//			U8_T week;
//			U8_T mon;
//			U8_T year;
//			U8_T century;	
//		}Clk;
//	}Time;							//8
} STR_MODBUS;

extern STR_MODBUS Modbus; 
extern u8 uart_dealwithTag;
extern u8 serial_receive_timeout_count;
extern u8 Uart_Rec_Buf[MAX_BUF]; 
extern u16 tick_ctr; 
extern u16 update_flash_status; 

void uart_serial_restart(void);
void init_main_send_com(void);
void modbus_init(void);
u8 checkData(void);
void internalDeal(u16 start_address);
void responseData(u16 address);
void init_crc16(void);  
void SoftReset(void); 
void print_test(void);
void rec_initial(void);

typedef enum
{
	MODBUS_SERIALNUMBER_LOWORD = 0,
	MODBUS_SERIALNUMBER_HIWORD = 2,
	MODBUS_VERSION_NUMBER_LO = 4,
	MODBUS_VERSION_NUMBER_HI,
	MODBUS_ADDRESS,
	MODBUS_PRODUCT_MODEL,
	MODBUS_HARDWARE_REV,    
	MODBUS_PIC_VERSION,
	MODBUS_ADDRESS_PLUG_N_PLAY = 10, 
	MODBUS_BASE_ADDRESS = 15,
	MODBUS_UPDATE_STATUS = 16,

	MODBUS_PNI0_MODE  = 100,//0x79
	MODBUS_PNI0_UPDATE_RATE,//0x92=1.7ms,0x93=3ms,0x94=7ms,0x95=13ms,0x96=27ms,0x97=55ms,0x98=110ms,0x99=220ms,0x9a=440ms,
	MODBUS_PNI0_GAIN_X,
	MODBUS_PNI0_GAIN_Y,
	MODBUS_PNI0_GAIN_Z,
	MODBUS_PNI0_FILTER_X,
	MODBUS_PNI0_FILTER_Y,
	MODBUS_PNI0_FILTER_Z, 
	MODBUS_PNI0_INPUT_L_X,
	MODBUS_PNI0_INPUT_H_X,
	MODBUS_PNI0_INPUT_L_Y,
	MODBUS_PNI0_INPUT_H_Y,
	MODBUS_PNI0_INPUT_L_Z,
	MODBUS_PNI0_INPUT_H_Z,






	MODBUS_PNI1_MODE =120,
	MODBUS_PNI1_UPDATE_RATE , 
	MODBUS_PNI1_GAIN_X,
	MODBUS_PNI1_GAIN_Y,
	MODBUS_PNI1_GAIN_Z,
	MODBUS_PNI1_FILTER_X,
	MODBUS_PNI1_FILTER_Y,
	MODBUS_PNI1_FILTER_Z, 
	MODBUS_PNI1_INPUT_L_X,
	MODBUS_PNI1_INPUT_H_X,
	MODBUS_PNI1_INPUT_L_Y,
	MODBUS_PNI1_INPUT_H_Y,
	MODBUS_PNI1_INPUT_L_Z,
	MODBUS_PNI1_INPUT_H_Z, 






	MODBUS_RESET_PNI_INPUT=140,
	MODBUS_RECORD_NUMBER_OPTO_OFF,
	MODBUS_OPTO_STATUS, 
	MODBUS_OPTO_TRIGGLE_NUMBER,
	MODBUS_PNI_RECORD_NUMBER,
	MODBUS_READ_PNI_CONFIG,
	
	
	
	
	MODBUS_IP_MODE = 150,
	MODBUS_MAC_ADDRESS_0,
	MODBUS_MAC_ADDRESS_1,
	MODBUS_MAC_ADDRESS_2,
	MODBUS_MAC_ADDRESS_3,
	MODBUS_MAC_ADDRESS_4,
	MODBUS_MAC_ADDRESS_5, 
	MODBUS_IP_ADDRESS_0,
	MODBUS_IP_ADDRESS_1,
	MODBUS_IP_ADDRESS_2,
	MODBUS_IP_ADDRESS_3,
	MODBUS_SUBNET_MASK_ADDRESS_0,
	MODBUS_SUBNET_MASK_ADDRESS_1,
	MODBUS_SUBNET_MASK_ADDRESS_2,
	MODBUS_SUBNET_MASK_ADDRESS_3,
	MODBUS_GATEWAY_ADDRESS_0,
	MODBUS_GATEWAY_ADDRESS_1,
	MODBUS_GATEWAY_ADDRESS_2,
	MODBUS_GATEWAY_ADDRESS_3,
	MODBUS_TCP_PORT,
	MODBUS_TCP_SERVER_ENABLE,
	MODBUS_BEEP_STATUS,
	MODBUS_RELAY_STATUS,
	MODBUS_RTC_HOUR,
	MODBUS_RTC_MIN,
	MODBUS_RTC_SEC, 
	MODBUS_RTC_YEAR, 
	MODBUS_RTC_MONTH,
	MODBUS_RTC_DATE,
	MODBUS_RTC_WEEK, 
	MODBUS_RTC_GHOST_ENABLE,
	MODBUS_SD_CARD_STATUS,
	MODBUS_BAUD_RATE,
	MODBUS_TEST,
	
   MODBUS_REGISTER_END,
} MODBUS_MAP;
 
typedef enum { 
	EEP_SERIALNUMBER_LOWORD,             
	EEP_SERIALNUMBER_HIWORD  =2,
	EEP_VERSION_NUMBER_LO    =4,
	EEP_VERSION_NUMBER_HI    =5,     
	EEP_ADDRESS              =6,              
	EEP_PRODUCT_MODEL        =7,           
	EEP_HARDWARE_REV         =8,           
	EEP_PIC_VERSION          =9,   
	EEP_INFO_BYTE          	 =10,   
	EEPROM_TEST_ADDRESS      =11,
	EEP_UPDATE_STATUS        =16,// registers needed for updating status
	EEP_UPDATE_PTR_HI,         
	EEP_UPDATE_PTR_LO, 
	
	EEP_PNI0_MODE,
	EEP_PNI0_UPDATE_RATE , 
	EEP_PNI0_GAIN_X,
	EEP_PNI0_GAIN_Y=EEP_PNI0_GAIN_X+2,
	EEP_PNI0_GAIN_Z=EEP_PNI0_GAIN_Y+2,
	EEP_PNI0_FILTER_X=EEP_PNI0_GAIN_Z+2,
	EEP_PNI0_FILTER_Y,
	EEP_PNI0_FILTER_Z,
	
	EEP_PNI1_MODE,
	EEP_PNI1_UPDATE_RATE , 
	EEP_PNI1_GAIN_X,
	EEP_PNI1_GAIN_Y=EEP_PNI1_GAIN_X+2,
	EEP_PNI1_GAIN_Z=EEP_PNI1_GAIN_Y+2,
	EEP_PNI1_FILTER_X=EEP_PNI1_GAIN_Z+2,
	EEP_PNI1_FILTER_Y,
	EEP_PNI1_FILTER_Z,
	EEP_RECORD_NUMBER_OPTO_OFF,
	EEP_IP_MODE,
	EEP_MAC_ADDRESS,  
	EEP_IP_ADDRESS = EEP_MAC_ADDRESS + 6, 
	EEP_SUBNET_MASK_ADDRESS = EEP_IP_ADDRESS  + 4,
	EEP_GATEWAY_ADDRESS = EEP_SUBNET_MASK_ADDRESS + 4,
	EEP_TCP_PORT = EEP_GATEWAY_ADDRESS + 4,
	EEP_BAUD_RATE = EEP_TCP_PORT + 4, 
	EEP_END
	 
	 
	 
}EEP_MAP;
  
extern u16 test[];

#endif


