#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"


#define MAX_MAIN_ITEMS	4

static  u8 const item_name[MAX_MAIN_ITEMS][18] = 
{
	"1.Sensor1 Config",
	"2.Sensor2 Config",
	"3.Miscellaneous",
	"4.About "
};

static  u8 const item[] = {Sensor1_Conf, Sensor2_Conf,MenuMisc,MenuAbout}; 

static u8 item_index = 0;
static u8 pre_item_index = 0;

void Main_init(void)
{
	u8 i;  
	Lcd_Full_Screen(0);
	for(i = 0; i < MAX_MAIN_ITEMS; i++)
	{
		if( i == item_index )
			Lcd_Show_String(i, 0, DISP_INV, (char *)item_name[i]);
		else
			Lcd_Show_String(i, 0, DISP_NOR, (char *)item_name[i]);
	} 	 
}

void Main_display(void)
{ 
	if(pre_item_index != item_index)
	{
		Lcd_Show_String(pre_item_index, 0, DISP_NOR, (char *)item_name[pre_item_index]);
		Lcd_Show_String(item_index, 0, DISP_INV, (char *)item_name[item_index]);
		pre_item_index = item_index;
	}  
}

void Main_keycope(u16 key_value)
{
//	start_back_light(backlight_keep_seconds);
	 
		switch(key_value )
		{
			case KEY_NON:
				// do nothing
				break;
			case KEY_UP_MASK:
				if(item_index)
					item_index--;
				else
					item_index = MAX_MAIN_ITEMS-1;
				break;
			case KEY_DOWN_MASK:
				item_index++;
				item_index %= MAX_MAIN_ITEMS;
				break;
			case KEY_LEFT_MASK:
				update_menu_state(MenuIdle);
				break;
			case KEY_RIGHT_MASK:
				// enter sub menu
				update_menu_state(item[item_index]);
				break; 		 
	}
}

