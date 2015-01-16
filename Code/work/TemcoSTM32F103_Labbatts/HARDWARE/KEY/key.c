#include "key.h"
#include "delay.h"
#include "stdio.h"
u8 global_key = KEY_NON;

//按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOC时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
} 

u8 KEY_Scan(void)
{	 
	u8 key_1st, key_2nd;
	u8 key_val = KEY_NON;
	
	key_1st = ~GPIO_ReadInputData(GPIOC) & 0xf; // PC0-3
	delay_ms(20);
	key_2nd = ~GPIO_ReadInputData(GPIOC) & 0xf; // PC0-3
	
	if(key_1st & key_2nd & KEY_1)
		key_val |= KEY_1;
	
	if(key_1st & key_2nd & KEY_2)
		key_val |= KEY_2;
	
	if(key_1st & key_2nd & KEY_3)
		key_val |= KEY_3;
	
	if(key_1st & key_2nd & KEY_4)
		key_val |= KEY_4;
	
	return key_val;
}
