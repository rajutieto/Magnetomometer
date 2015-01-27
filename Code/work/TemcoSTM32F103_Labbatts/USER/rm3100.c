#include "rm3100.h"
#include "modbus.h"
#include "led.h"

u8 pre_alarm_on_time = 2;
u8 pre_alarm_off_time = 2;

_RM3100_ rm3100[2];  
_PRINT_BUF_ print_buf[REC_TIMER_TOTAL+1];
_PRINT_BUF_ tcp_buf[REC_TIMER_TOTAL+1];

 /*

s16 clbr_value[10][2];

 
float slope_count()
{
	u8   i;
	float  X_all, Y_all,XY_all,X2_all,k_line;  
	
	X_all=clbr_value[0][VOLT]; 

	for(i=1;i<POINTS_ALL;i++)
	{
		X_all+=(clbr_value[i][VOLT] );	
	}
	 

	X2_all=(clbr_value[0][VOLT])*(clbr_value[0][VOLT]);
	for(i=1;i<POINTS_ALL;i++)
	{
		X2_all+=(clbr_value[i][VOLT]*clbr_value[i][VOLT]);		
	}
	 
	Y_all=clbr_value[0][PR]; 
	for(i=1;i<POINTS_ALL;i++)
	{
		Y_all+=clbr_value[i][PR];	
	}	
	 
	XY_all=clbr_value[0][PR]*(clbr_value[0][VOLT]);
	for(i=1;i<POINTS_ALL;i++)
	{
		XY_all+=(clbr_value[i][PR]*clbr_value[i][VOLT]);
	}
	 
	k_line=(XY_all-(Y_all*X_all)/POINTS_ALL)/(X2_all-(X_all*X_all)/POINTS_ALL);
	//b_line=(XY_all-(X2_all*k_line))/X_all;
 
}
*/
extern u16 OVER_REC_TIMER;
void rm3100_initial(void)
{
	u8 i, j;
	for(i=0;i<2;i++)
	{
		for(j=0;j<TOTAL_AXIS;j++)
		{  
			rm3100[i].input[j] = 0;
			rm3100[i].preinput[j] = 0;
			rm3100[i].offset[j] = 0;	
		}
		rm3100[i].filter = 10;
		rm3100[i].sensitivity = 100;
		rm3100[i].result = 0;
		rm3100[i].triggle_number = 0; 
		rm3100[i].data_number = 0; 
		rm3100[i].result = 0; 
	} 
	rec_timer = 0;
	OVER_REC_TIMER = START_REC_TIMER;
}

void rm3100_reset(void)
{
	rm3100[0].result = 0; 
	rm3100[1].result = 0;
	LED0 = 1;
}

void rm3100_detect(void)
{
	u16 sampling_counter,i;
	s32 min,max;  
	//sensor1
	for(i=0;i<TOTAL_AXIS;i++)
	{
		min = print_buf[START_REC_TIMER].RM3100_0[i];
		max = print_buf[START_REC_TIMER].RM3100_0[i];
		   
		for(sampling_counter = START_REC_TIMER; sampling_counter < rm3100[0].data_number+START_REC_TIMER; sampling_counter++)
		{         
			min = min < print_buf[sampling_counter].RM3100_0[i]? min : print_buf[sampling_counter].RM3100_0[i];
			max = max > print_buf[sampling_counter].RM3100_0[i]? max : print_buf[sampling_counter].RM3100_0[i];    
		}
		if((max - min)>rm3100[0].sensitivity) rm3100[0].result |= (1<<i);
	}
	//sensor2
	for(i=0;i<TOTAL_AXIS;i++)
	{
		min = print_buf[START_REC_TIMER].RM3100_1[i];
		max = print_buf[START_REC_TIMER].RM3100_1[i];
		   
		for(sampling_counter = START_REC_TIMER; sampling_counter < rm3100[1].data_number+START_REC_TIMER; sampling_counter++)
		{         
			min = min < print_buf[sampling_counter].RM3100_1[i]? min : print_buf[sampling_counter].RM3100_1[i];
			max = max > print_buf[sampling_counter].RM3100_1[i]? max : print_buf[sampling_counter].RM3100_1[i];    
		}
		if((max - min)>rm3100[1].sensitivity) rm3100[1].result |= (1<<i);; 		
		 
	}
}

void  rm3100_clear(void)
{
	u8 i, j;
	for(i=0;i<2;i++)
	{
		for(j=0;j<TOTAL_AXIS;j++)
		{    
			rm3100[i].offset[j] = rm3100[i].input[j] ;	
		} 
		rm3100[i].triggle_number = 0;
	} 	
}

