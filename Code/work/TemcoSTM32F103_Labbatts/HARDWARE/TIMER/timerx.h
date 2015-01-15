#ifndef __TIMERX_H
#define __TIMERX_H

#include "stm32f10x.h"


void TIM3_Int_Init(u16 arr, u16 psc);
//void TIM3_PWM_Init(u16 arr, u16 psc);

//void TIM4_Int_Init(void);

extern u32 uip_timer;
void TIM6_Int_Init(u16 arr, u16 psc);

#endif
