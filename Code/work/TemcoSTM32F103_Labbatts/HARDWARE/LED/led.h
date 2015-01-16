#ifndef __LED_H
#define __LED_H
 
#include "bitmap.h"

#define LED_ON	0
#define LED_OFF	1	

#define LED0	PAout(0)// DS0
#define LED1	PAout(2)// DS1
#define LED2	PAout(4)// DS2
#define LED3	PAout(6)// DS3

// DS0
#define LED0_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_0);
#define LED0_ON()	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
// DS1
#define LED1_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_2);
#define LED1_ON()	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
// DS2
#define LED2_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_4);
#define LED2_ON()	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
// DS3
#define LED3_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define LED3_ON()	GPIO_ResetBits(GPIOA, GPIO_Pin_6);


void LED_Init(void);
		 				    
#endif

















