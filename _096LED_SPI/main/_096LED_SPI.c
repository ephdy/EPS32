#include <stdio.h>
#include <string.h>
#include "OLED_SPI.h"



void app_main(void)
{
    DC_Command;
    oled_init();
    //oled_write_byte(0xa5);

    /*
    for (uint8_t i = 0; i < 8; i++)
    {
        oled_set_cursor(i,0);
        DC_Data;
        for (uint8_t j = 0; j < 128; j++)
        {
            oled_write_byte(0x00);
	    }
    }
    */
    
    oled_clr();
    oled_witre_string(0,0,"I LOVE LIUDAN!",8);
}