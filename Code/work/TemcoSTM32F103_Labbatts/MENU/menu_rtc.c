#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"
#include "rm3100.h"
#include "rtc.h"

static u8 item_index = 0;
static u8 pre_item_index = 0;

static u16 set_value;

enum
{
	YEAR=0,
	MONTH,
	DATA,
	HOUR,
	MIN, 
};
#define MAX_TEMP_ITEMS	5
static u8 const code item_name[MAX_TEMP_ITEMS][17] = 
{
	"1.time: ",  
};
//1.timer:
//15-01-06 12:22

void RTC_Conf_init(void)
{
	u8 i,pos_line;
	Lcd_Full_Screen(0);
	sprintf((char *)text, "%s", item_name[0]);
	Lcd_Show_String(0, 0, DISP_INV, (char *)text);
	sprintf((char *)text, "%2u-%2u-%2u %2u:%2u", calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min);
	Lcd_Show_String(1, 0, DISP_NOR, (char *)text);
//	for(i = 0; i < MAX_TEMP_ITEMS; i++)
//	{
//		
//		switch(i)
//		{
//			
//			case YEAR: // year 
//				sprintf((char *)text, "%2u", calendar.w_year);	
//				pos_line = 0;
//			break; 	
//			case MONTH: // year 
//				sprintf((char *)text, "%2u", calendar.w_month);				 
//			break;
//			case DATA: // year 
//				sprintf((char *)text, "%2u", calendar.w_date);				 
//			break;
//			case HOUR: // year 
//				sprintf((char *)text, "%2u", calendar.hour);				 
//			break;
//			case MIN: // year 
//				sprintf((char *)text, "%2u", calendar.min);				 
//			break; 
//		}

//		if(i == item_index)
//			Lcd_Show_String(i, 0, DISP_INV, (char *)text);
//		else
//			Lcd_Show_String(i, 0, DISP_NOR, (char *)text);
//	}

	in_sub_menu = FALSE;
}

void RTC_Conf_display(void)
{
	u8 pos_line;
	if(in_sub_menu == FALSE)
	{
		if(pre_item_index != item_index)
		{
			switch(pre_item_index)
			{  
				case YEAR: // year 
					sprintf((char *)text, "%2u", calendar.w_year);	
					pos_line = 0;
				break; 	
				case MONTH: // year 
					sprintf((char *)text, "%2u", calendar.w_month);
					pos_line = 3;
				break;
				case DATA: // year 
					sprintf((char *)text, "%2u", calendar.w_date);
					pos_line = 6;
				break;
				case HOUR: // year 
					sprintf((char *)text, "%2u", calendar.hour);
					pos_line = 9;
				break;
				case MIN: // year 
					sprintf((char *)text, "%2u", calendar.min);	
					pos_line = 12;
				break; 	  
			} 
			Lcd_Show_String(1, pos_line, DISP_NOR, (char *)text);

			switch(item_index)
			{
				case YEAR: // year 
					sprintf((char *)text, "%2u", calendar.w_year);	
					pos_line = 0;
				break; 	
				case MONTH: // year 
					sprintf((char *)text, "%2u", calendar.w_month);
					pos_line = 3;
				break;
				case DATA: // year 
					sprintf((char *)text, "%2u", calendar.w_date);
					pos_line = 6;
				break;
				case HOUR: // year 
					sprintf((char *)text, "%2u", calendar.hour);
					pos_line = 9;
				break;
				case MIN: // year 
					sprintf((char *)text, "%2u", calendar.min);	
					pos_line = 12;
				break; 
				 
			} 
			Lcd_Show_String(1, pos_line, DISP_INV, (char *)text);

			pre_item_index = item_index;
		}
		 
	}
	else
	{
		if(value_change == TRUE)
		{
			switch(item_index)
			{ 
				case YEAR: // year 
					sprintf((char *)text, "%2u", set_value);	
					pos_line = 0;
				break; 	
				case MONTH: // year 
					sprintf((char *)text, "%2u", set_value);
					pos_line = 3;
				break;
				case DATA: // year 
					sprintf((char *)text, "%2u", set_value);
					pos_line = 6;
				break;
				case HOUR: // year 
					sprintf((char *)text, "%2u", set_value);
					pos_line = 9;
				break;
				case MIN: // year 
					sprintf((char *)text, "%2u", set_value);	
					pos_line = 12;
				break; 
				Lcd_Show_String(1, pos_line, DISP_NOR, (char *)text);
			}
			
			value_change = FALSE;	
		}
	}
}

void RTC_Conf_keycope(u16 key_value)
{
//	start_back_light(backlight_keep_seconds);
	u8 pos_line;
	/*
	switch(key_value )
	{
		case KEY_NON:
			// do nothing
			break;
		case KEY_LEFT_MASK:
			if(in_sub_menu == FALSE)
			{
				update_menu_state(MenuMain);
			}
			else
			{
				sprintf((char *)text, "%s", item_name[0]);
				Lcd_Show_String(0, 0, DISP_INV, (char *)text);
				sprintf((char *)text, "%2u-%2u-%2u %2u:%2u", calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min);
				Lcd_Show_String(1, 0, DISP_NOR, (char *)text);
				in_sub_menu = FALSE;
			}
			break;
		case KEY_RIGHT_MASK:
			if(in_sub_menu == FALSE)
			{ 
				switch(item_index)
				{
					  
					case YEAR:  
						set_value = calendar.w_year;
						sprintf((char *)text, "%2u", calendar.w_year);	
						pos_line = 0;
					break; 	
					case MONTH:  
						set_value = calendar.w_month;
						sprintf((char *)text, "%2u", calendar.w_month);
						pos_line = 3;
					break;
					case DATA: 
						set_value = calendar.w_date;
						sprintf((char *)text, "%2u", calendar.w_date);
						pos_line = 6;
					break;
					case HOUR:  
						set_value = calendar.hour;
						sprintf((char *)text, "%2u", calendar.hour);
						pos_line = 9;
					break;
					case MIN:  
						set_value = calendar.min;
						sprintf((char *)text, "%2u", calendar.min);	
						pos_line = 12;
					break; 
					Lcd_Show_String(1, pos_line, DISP_INV, (char *)text); 	 
				}
			
				in_sub_menu = TRUE;
			}
			else
			{
				switch(item_index)
				{
					  
					case YEAR:  
						set_value = calendar.w_year;
						sprintf((char *)text, "%2u", calendar.w_year);	
						pos_line = 0;
					break; 	
					case MONTH:  
						set_value = calendar.w_month;
						sprintf((char *)text, "%2u", calendar.w_month);
						pos_line = 3;
					break;
					case DATA: 
						set_value = calendar.w_date;
						sprintf((char *)text, "%2u", calendar.w_date);
						pos_line = 6;
					break;
					case HOUR:  
						set_value = calendar.hour;
						sprintf((char *)text, "%2u", calendar.hour);
						pos_line = 9;
					break;
					case MIN:  
						set_value = calendar.min;
						sprintf((char *)text, "%2u", calendar.min);	
						pos_line = 12;
					break; 
					Lcd_Show_String(1, pos_line, DISP_INV, (char *)text); 	 
				}
				item_index++;
				if(item_index == MAX_TEMP_ITEMS)
				{
					item_index = 0;
					switch(item_index)
					{ 
						case 0: // filter 
							rm3100[1].filter = set_value ;
							sprintf((char *)text, "%s%2u", item_name[0], set_value); 
						break;
						 
						 
					}
					sprintf((char *)text, "%s", item_name[0]);
					Lcd_Show_String(0, 0, DISP_INV, (char *)text);
					sprintf((char *)text, "%2u-%2u-%2u %2u:%2u", calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min);
					Lcd_Show_String(1, 0, DISP_NOR, (char *)text);

					in_sub_menu = FALSE;
				}
			}
			break;
		case KEY_UP_MASK:
			if(in_sub_menu == FALSE)
			{
				if(item_index)
					item_index--;
				else
					item_index = MAX_TEMP_ITEMS - 1;
			}
			else
			{
				switch(item_index)
				{
					case 0:
						 if(set_value < 100) set_value++;
					break;
					case 1:
						if(set_value < 1000)
							set_value+=10; 						
						break;
					 
				}

				value_change = TRUE;
			}
			break;
		case KEY_DOWN_MASK:
			if(in_sub_menu == FALSE)
			{
				item_index++;
				item_index %= MAX_TEMP_ITEMS;
			}
			else
			{
				switch(item_index)
				{
					case 0:
						if(set_value)
							set_value--; 						
						break;
					case 1:
						if(set_value)
							set_value-=10; 						
						break;
					 
				}
				value_change = TRUE;
			}
			break;
	}
	*/
}

