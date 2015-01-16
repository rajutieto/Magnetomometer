#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"
#include "rm3100.h"

static u8 item_index = 0;
static u8 pre_item_index = 0;

static u16 set_value;

#define MAX_TEMP_ITEMS	2
static u8 const code item_name[MAX_TEMP_ITEMS][17] = 
{
	"1.Filter: ",
	"2.Sensitivity: ",  
};

void Sensor1_Conf_init(void)
{
	u8 i;
	Lcd_Full_Screen(0);
	for(i = 0; i < MAX_TEMP_ITEMS; i++)
	{
		switch(i)
		{
			case 0: // filter 
				sprintf((char *)text, "%s%2u", item_name[i], rm3100[0].filter);				 
			break;
			case 1:	// sensitivity 
				sprintf((char *)text, "%s%3u", item_name[i], rm3100[0].sensitivity); 	
			break; 	 
		}

		if(i == item_index)
			Lcd_Show_String(i, 0, DISP_INV, (char *)text);
		else
			Lcd_Show_String(i, 0, DISP_NOR, (char *)text);
	}

	in_sub_menu = FALSE;
}

void Sensor1_Conf_display(void)
{
	if(in_sub_menu == FALSE)
	{
		if(pre_item_index != item_index)
		{
			switch(pre_item_index)
			{  
				case 0: // filter 
					sprintf((char *)text, "%s%2u", item_name[pre_item_index], rm3100[0].filter);				 
				break;
				case 1:	// sensitivity 
					sprintf((char *)text, "%s%3u", item_name[pre_item_index], rm3100[0].sensitivity); 	
				break; 	  
			}
			Lcd_Clear_Row(pre_item_index);
			Lcd_Show_String(pre_item_index, 0, DISP_NOR, (char *)text);

			switch(item_index)
			{
				case 0: // filter 
					sprintf((char *)text, "%s%2u", item_name[item_index], rm3100[0].filter);				 
				break;
				case 1:	// sensitivity 
					sprintf((char *)text, "%s%3u", item_name[item_index], rm3100[0].sensitivity); 	
				break; 
				 
			}
			Lcd_Clear_Row(item_index);
			Lcd_Show_String(item_index, 0, DISP_INV, (char *)text);

			pre_item_index = item_index;
		}
		 
	}
	else
	{
		if(value_change == TRUE)
		{
			switch(item_index)
			{
				 
				case 0: // filter  
					sprintf((char *)text, "%2u", set_value);
					Lcd_Show_String(item_index, 10, DISP_INV, (char *)text);
				break;
				case 1:	// sensitivity   
					sprintf((char *)text, "%3u", set_value);
					Lcd_Show_String(item_index, 15, DISP_INV, (char *)text);					
				break; 
			}
			
			value_change = FALSE;	
		}
	}
}

void Sensor1_Conf_keycope(u16 key_value)
{
//	start_back_light(backlight_keep_seconds);
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
				switch(item_index)
				{
					case 0: // filter 
						sprintf((char *)text, "%s%2u", item_name[item_index], rm3100[0].filter);				 
					break;
					case 1:	// sensitivity 
						sprintf((char *)text, "%s%3u", item_name[item_index], rm3100[0].sensitivity); 	
					break; 
					 
				}
				Lcd_Show_String(item_index, 0, DISP_INV, (char *)text); 
				in_sub_menu = FALSE;
			}
			break;
		case KEY_RIGHT_MASK:
			if(in_sub_menu == FALSE)
			{
				Lcd_Show_String(item_index, 0, DISP_NOR, (char *)item_name[item_index]);
				switch(item_index)
				{
					 
					case 0: // filter 
						set_value = rm3100[0].filter;
						sprintf((char *)text, "%2u", set_value);
						Lcd_Show_String(item_index, 10, DISP_INV, (char *)text);
					break;
					case 1:	// sensitivity  
						set_value = rm3100[0].sensitivity;
						sprintf((char *)text, "%3u", set_value);
						Lcd_Show_String(item_index, 15, DISP_INV, (char *)text);					
					break; 
					 
				}
			
				in_sub_menu = TRUE;
			}
			else
			{
				switch(item_index)
				{ 
					case 0: // filter 
						rm3100[0].filter = set_value ;
						sprintf((char *)text, "%s%2u", item_name[item_index], set_value); 
					break;
					case 1:	// sensitivity  
						rm3100[0].sensitivity = set_value;
						sprintf((char *)text, "%s%3u", item_name[item_index], set_value); 					
					break; 
					 
				}
				Lcd_Show_String(item_index, 0, DISP_INV, (char *)text);

				in_sub_menu = FALSE;
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
}

