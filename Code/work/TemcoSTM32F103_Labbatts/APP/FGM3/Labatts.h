#ifndef __LABATTS_H

#define __LABATTS_H

#include "stm32f10x.h"

typedef struct 
{
	u8 status;
	s16 ch1;
	s16 ch2;
} _FGM3_;

extern _FGM3_ fgm3;
extern u32 beer_counter;

typedef struct
{
	u16 val;
	u16 sensitivity;
} _OPTO_;
extern _OPTO_ opto;

extern u8 opto_capture ;

#define FILTER_TIMES	5

void FGM3_Init(void);
void FGM3_ISR(void);
void FGM3_enable_read(void);

//u8 get_beer_counter(void);

u8 read_pic(void);
u8 write_pic(u16 sensitivity);


#endif
