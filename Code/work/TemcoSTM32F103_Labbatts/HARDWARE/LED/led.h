#ifndef __LED_H
#define __LED_H
 
#include "bitmap.h"

#define LED_ON	0
#define LED_OFF	1	

#define LED0	PCout(4)// DS0
#define LED1	PCout(5)// DS1
#define LED2	PCout(6)// DS2
#define LED3	PCout(7)// DS3

#define RELAY_CONTROL	PDout(0)// Relay
#define BEEP_CONTROL	PBout(9)// Beep

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

















