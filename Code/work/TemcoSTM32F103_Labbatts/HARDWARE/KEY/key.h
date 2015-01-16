#ifndef __KEY_H
#define __KEY_H
	 
#include "bitmap.h"

#define KEY_NON		0X00
#define	KEY_1		0X01	//PC0
#define	KEY_2		0X02	//PC1
#define	KEY_3		0X04	//PC2
#define	KEY_4		0X08	//PC3 

#define	KEY_UP_MASK			(1 << 3)
#define	KEY_DOWN_MASK		(1 << 2)
#define	KEY_LEFT_MASK		(1 << 1)
#define	KEY_RIGHT_MASK		(1 << 0)

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数

extern u8 global_key;
					    
#endif
