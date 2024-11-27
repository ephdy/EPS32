#ifndef RMT_WS2812_H
#define RMT_WS2812_H

#include "WS2812.h"


led_strip_handle_t configure_led(void);
void WS2812_Updata(led_strip_handle_t led_strip,bool led_on_off,uint32_t *ARGB,uint8_t brightness);

#endif