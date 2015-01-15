#ifndef _RM3100_H_
#define _RM3100_H_
#include "bitmap.h"

 
void rm3100_detect(void);
void rm3100_initial(void);
void rm3100_reset(void);
void  rm3100_clear(void);

#define REC_TIMER_TOTAL             500
#define START_REC_TIMER             50
#define END_REC_TIMER               50

#define TOTAL_AXIS					3
#define X_AXIS						0
#define Y_AXIS						1
#define Z_AXIS						2




typedef struct 
{
	s32 input[TOTAL_AXIS];
	s32 preinput[TOTAL_AXIS];
    s32 offset[TOTAL_AXIS];	
	u8  filter;
	u16 data_number;
	u16 sensitivity;
	u8 result;
	u32 triggle_number;
	
} _RM3100_;

typedef struct 
{
	s32 RM3100_0[TOTAL_AXIS];
	s32 RM3100_1[TOTAL_AXIS];
	u16 FGM_1;
	u16 FGM_2;
	u8  opto_status;
} _PRINT_BUF_;

extern _PRINT_BUF_ print_buf[REC_TIMER_TOTAL+1];

extern _RM3100_ rm3100[2]; 

#endif

