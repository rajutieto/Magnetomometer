#ifndef _RM3100_H_
#define _RM3100_H_
#include "bitmap.h"
#include "FreeRTOS.h"
#include "task.h"
 

#define MAX_RECORD_BUFFER             300
 

#define TOTAL_AXIS					3
#define X_AXIS						0
#define Y_AXIS						1
#define Z_AXIS						2

#define OFF 						0
#define ON							1

 typedef struct 
{ 
	u8  Mode;   		 		//0x79
	u8  Update_Rate;  			//0x92=1.7ms,0x93=3ms,0x94=7ms,0x95=13ms,0x96=27ms,0x97=55ms,0x98=110ms,0x99=220ms,0x9a=440ms,
	u16 Gian[TOTAL_AXIS]; 		
	s32 InPut[TOTAL_AXIS];  
	s32 PreInput[TOTAL_AXIS];
    s32 OffSet[TOTAL_AXIS];	
	u8  Filter[TOTAL_AXIS]; 
}STR_MAGNETIC;

extern STR_MAGNETIC pni[]; 



#pragma pack(push)
#pragma pack(1)
typedef struct 
{
	s32 Sensor0[TOTAL_AXIS];
	s32 Sensor1[TOTAL_AXIS]; 
	u8  Opto_Status;
} STR_PNI_BUF;
#pragma pack(pop)
#define PNI_BUF_SIZE		sizeof(STR_PNI_BUF) 
extern STR_PNI_BUF pni_buffer[];
extern STR_PNI_BUF TCP_Buffer[];


typedef struct
{
	u8  reset_input_flag;
	u16 Record_Number_Opto_Off;
	u8  record_over_flag;
	u16 pni_record_number;
	u16 opto_triggle_number;
	u8  opto_status;
	u8 read_config_flag;
	
}STR_PNI_PARA;

extern STR_PNI_PARA  pni_para;



void vStartPNITask( portBASE_TYPE uxPriority);

 
#endif

