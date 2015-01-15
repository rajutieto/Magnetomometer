#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"
#include "delay.h"
u8 text[150];
u8 menu_system_start = FALSE;
u32 menu_block_timer_start, menu_block_timer_end;

struct _MENU_STATE_ CurrentState;
struct _DISPLAY_ display;
u8 in_sub_menu = FALSE;
u8 value_change = FALSE;

struct _MENU_STATE_ const code StateArray[MenuEnd] = 
{
	{
		MenuIdle,
		Idle_keycope,
		Idle_init,
		Idle_display,
		0,
	},
			{
				MenuMain,
				Main_keycope,
				Main_init,
				Main_display,
				MENU_BLOCK_SECONDS_DEFAULT,
			},
		
					{
						Sensor1_Conf,
						Sensor1_Conf_keycope,
						Sensor1_Conf_init,
						Sensor1_Conf_display,
						MENU_BLOCK_SECONDS_DEFAULT,
					},
					{
						Sensor2_Conf,
						Sensor2_Conf_keycope,
						Sensor2_Conf_init,
						Sensor2_Conf_display,
						MENU_BLOCK_SECONDS_DEFAULT,
					},
					{
						RTC_Conf,
						RTC_Conf_keycope,
						RTC_Conf_init,
						RTC_Conf_display,
						MENU_BLOCK_SECONDS_DEFAULT,
					},
					{
						MenuAbout,
						About_keycope,
						About_init,
						About_display,
						MENU_BLOCK_SECONDS_DEFAULT,
					},
 
}; 
 
void show_system_info(void)
{

			display.x1 = 0;
			display.x2 = 0;
			display.y1 = 0;
			display.y2 = 0;
			display.z1 = 0;
			display.z2 = 0;  
			display.left_result  = 0;
			display.right_result = 0;
			display.triggle_number = 0;
	
	
} 


void update_menu_state(u8 MenuId)
{
//	if(MenuId == MenuIdle)
//	{
//		start_scrolling();
//	}
//	else
//	{
//		stop_scrolling();
//		menu_block_timer_start = SWTIMER_Tick();
//	}

	memcpy((void *)&CurrentState, (void *)&StateArray[MenuId], sizeof(struct _MENU_STATE_));
	CurrentState.InitAction();
}
void menu_init(void)
{
//	u16 key_temp;
	// clear the key events which happened within the menu init routine
//	while(cQueueReceive(qKey, &key_temp, 0) == pdTRUE)
//	{
//		watchdog_reset();
//	}

	menu_system_start = TRUE;
	update_menu_state(MenuIdle);
}

 void MenuTask(void *pvParameters)
{
	u16 key_temp;
//	u8 i;
//	portTickType xDelayPeriod = (portTickType)100 / portTICK_RATE_MS;
	Lcd_Initial();
 	show_system_info();
//	for(i = 0; i < 3; i++)
//	{
//		vTaskDelay(100);
//	}

	
	menu_init();
	while(1)
	{
//		vTaskDelay(xDelayPeriod);
		if(global_key != KEY_NON)
		{
			key_temp = global_key;
			global_key = KEY_NON;

			CurrentState.KeyCope(key_temp);
			if(CurrentState.BlockTime)
				menu_block_timer_start = xTaskGetTickCount();	
		}

//		if(cQueueReceive(qKey, &key_temp, 20) == pdTRUE)
//		{
//			CurrentState.KeyCope(key_temp);
//			if(CurrentState.BlockTime)
//				menu_block_timer_start = xTaskGetTickCount();
//		}

		if(menu_system_start == TRUE)
		{		
			CurrentState.DisplayPeriod();
			if(CurrentState.BlockTime)
			{
				menu_block_timer_end = xTaskGetTickCount();
				if((menu_block_timer_end - menu_block_timer_start) >= (CurrentState.BlockTime * SWTIMER_COUNT_SECOND))
					update_menu_state(MenuIdle);
			}
		}
		
		delay_ms(100); 		
	}	
}

void vStartMenuTask( portBASE_TYPE uxPriority)
{ 
	xTaskCreate( MenuTask, ( signed portCHAR * ) "MenuTask", configMINIMAL_STACK_SIZE, NULL, uxPriority , NULL );	 	
}

