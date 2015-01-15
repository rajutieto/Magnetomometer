#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"
#include "rm3100.h"
void Idle_init(void)
{
	Lcd_Full_Screen(0);
	Lcd_Show_String(0, 0, DISP_NOR, "X1:        X2:       ");
	Lcd_Show_String(1, 0, DISP_NOR, "Y1:        Y2:       ");
	Lcd_Show_String(2, 0, DISP_NOR, "Z1:        Z2:       "); 
	Lcd_Show_String(3, 0, DISP_NOR, "number:              "); 
	Lcd_Show_String(4, 0, DISP_NOR, "metal :              ");
}

void Idle_display(void)
{
	   
		sprintf((char *)text, "%6d",  display.x1);
		Lcd_Show_String(0, 3, DISP_NOR, (char *)text);
		sprintf((char *)text, "%6d",  display.x2);
		Lcd_Show_String(0, 14, DISP_NOR, (char *)text); 
		sprintf((char *)text, "%6d",  display.y1);
		Lcd_Show_String(1, 3, DISP_NOR,(char *)text);
		sprintf((char *)text, "%6d",  display.y2);
		Lcd_Show_String(1, 14, DISP_NOR, (char *)text); 
		sprintf((char *)text, "%6d",  display.z1);
		Lcd_Show_String(2, 3, DISP_NOR, (char *)text);
		sprintf((char *)text, "%6d",  display.z2);
		Lcd_Show_String(2, 14, DISP_NOR, (char *)text);

		sprintf((char *)text, "%7d",  display.triggle_number);
		Lcd_Show_String(3, 8, DISP_NOR, (char *)text);

		sprintf((char *)text, "%3d",  display.left_result);
		Lcd_Show_String(4, 8, DISP_NOR, (char *)text);
		sprintf((char *)text, "%3d",  display.right_result);
		Lcd_Show_String(4, 16, DISP_NOR, (char *)text);
			 
}

void Idle_keycope(u16 key_value)
{
//	start_back_light(backlight_keep_seconds);
	switch(key_value )
	{
		case KEY_NON:
			// do nothing
			break;
		case KEY_UP_MASK:
			rm3100_reset();
			break;
		case KEY_DOWN_MASK:
			rm3100_clear();
			break;
		case KEY_LEFT_MASK:
			// do nothing
			break;
		case KEY_RIGHT_MASK:
			// go into main menu
			update_menu_state(MenuMain);
			break;
	}
}

