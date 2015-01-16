#include <string.h>
#include <stdio.h> 
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../key/key.h"


u8 const code product_name[] = "MetalDetect";
u8 const code vender[] = "www.temcocontrols.com";

void About_init(void)
{
	Lcd_Full_Screen(0);
	Lcd_Show_String(1, 2, DISP_NOR, (char *)product_name);
	Lcd_Show_String(3, 0, DISP_NOR, (char *)vender);
}

void About_display(void)
{
	
}

void About_keycope(u16 key_value)
{
//	start_back_light(backlight_keep_seconds);
	if((key_value ) != KEY_NON)
		update_menu_state(MenuMain);
}


