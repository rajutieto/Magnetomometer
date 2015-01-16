#ifndef _MENU_H_
#define _MENU_H_
#include "bitmap.h"
#include "FreeRTOS.h"
#include "task.h"
#define MENU_BLOCK_SECONDS_DEFAULT		60
//#define BACKLIGHT_KEEP_SECONDS_DEFAULT	30
#define SWTIMER_COUNT_SECOND         100
enum
{
	MenuIdle = 0,
		MenuMain,
			Sensor1_Conf,
			Sensor2_Conf,
			RTC_Conf,
			MenuAbout,
  
	MenuEnd,
};
struct _MENU_STATE_
{
	u8 Index;
	void (*KeyCope)(u16);								
	void (*InitAction)(void);
	void (*DisplayPeriod)(void);
	u32 BlockTime;
};

struct _DISPLAY_
{
	s32 x1;
	s32 y1; 
	s32 z1;
	s32 x2;
	s32 y2;
	s32 z2;
	u32 triggle_number;
	u8  right_result;
	u8  left_result;
	
};

extern struct _MENU_STATE_ CurrentState;
extern struct _DISPLAY_ display;
extern u8 in_sub_menu;
extern u8 value_change;
extern u8 text[150];

void vStartMenuTask(portBASE_TYPE uxPriority);
void update_menu_state(u8 MenuId);

//menu idle.c
void Idle_init(void);
void Idle_display(void);
void Idle_keycope(u16 key_value);

//menu main .c 
void Main_init(void);
void Main_display(void);
void Main_keycope(u16 key_value);

//menu sensor1 config .c
void Sensor1_Conf_init(void);
void Sensor1_Conf_display(void);
void Sensor1_Conf_keycope(u16 key_value);

//menu sensor2 config .c
void Sensor2_Conf_init(void);
void Sensor2_Conf_display(void);
void Sensor2_Conf_keycope(u16 key_value);

//menu rtc.c
void RTC_Conf_init(void);
void RTC_Conf_display(void);
void RTC_Conf_keycope(u16 key_value);

//menu about .c 
void About_init(void);
void About_display(void);
void About_keycope(u16 key_value);




#endif
