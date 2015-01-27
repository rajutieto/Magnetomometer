#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"
#include "delay.h"
#include "tcp_demo.h"
u8 text[150];
u8 menu_system_start = FALSE;
u32 menu_block_timer_start, menu_block_timer_end;
 
uint8 menu_password = FALSE;
uint8 use_password = FALSE;
uint8 password_buffer[4];
uint8 user_password[4] = {'1', '2', '3', '4'};
uint8 password_index;

uint32 menu_block_timer_start, menu_block_timer_end;
uint8 menu_block_seconds = MENU_BLOCK_SECONDS_DEFAULT;
uint8 backlight_keep_seconds = BACKLIGHT_KEEP_SECONDS_DEFAULT;

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
						MenuMisc,
						Misc_keycope,
						Misc_init,
						Misc_display,
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
	
	sprintf((char *)text, "IP:   %u.%u.%u.%u", (u16)ip_address[0], (u16)ip_address[1], (u16)ip_address[2], (u16)ip_address[3]);
	Lcd_Show_String(0, 0, DISP_NOR, (char *)text);
	// subnet mask address
	sprintf((char *)text, "MASK: %u.%u.%u.%u", (u16)subnet_mask_address[0], (u16)subnet_mask_address[1], (u16)subnet_mask_address[2], (u16)subnet_mask_address[3]);
	Lcd_Show_String(1, 0, DISP_NOR, (char *)text);
	// tcp port
	sprintf((char *)text, "GATE: %u.%u.%u.%u", (u16)gateway_address[0], (u16)gateway_address[1], (u16)gateway_address[2], (u16)gateway_address[3]);
	Lcd_Show_String(2, 0, DISP_NOR, (char *)text);
	// tcp port
	sprintf((char *)text, "PORT: %u", ((u16)listen_port_at_tcp_server_mode[0] << 8) | listen_port_at_tcp_server_mode[1]);
	Lcd_Show_String(3, 0, DISP_NOR, (char *)text);
	// MAC address
	sprintf((char *)text, "MAC:%02X:%02X:%02X:%02X:%02X:%02X", (u16)mac_address[0], (u16)mac_address[1], (u16)mac_address[2], (u16)mac_address[3], (u16)mac_address[4], (u16)mac_address[5]);
	Lcd_Show_String(4, 0, DISP_NOR, (char *)text);
	
	
	
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
	u8 i;
//	portTickType xDelayPeriod = (portTickType)100 / portTICK_RATE_MS;
	Lcd_Initial();
 	show_system_info();
	for(i = 0; i < 3; i++)
	{
		vTaskDelay(100);
	}

	
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

