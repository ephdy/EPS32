#ifndef OLED_SPI_H
#define OLED_SPI_H

#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#define PIN_NUM_DC   2
#define PIN_NUM_D    1
#define RES_ON       gpio_set_level(GPIO_NUM_4,0)
#define RES_OFF      gpio_set_level(GPIO_NUM_4,1)
#define DC_Command   gpio_set_level(PIN_NUM_DC,0)
#define DC_Data      gpio_set_level(PIN_NUM_DC,1)    

#define OLED_HOST    SPI3_HOST

extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

void spi_init();
void oled_init();
void oled_write_byte(uint8_t chr);
void oled_write_bytes(uint8_t chr[],uint8_t n);
void oled_reset();
void oled_set_cursor(uint8_t Page,uint8_t Coul);
void oled_clr();
void oled_witre_char(uint8_t Page,uint8_t Coul,uint8_t chr,uint8_t width);
void oled_witre_string(uint8_t Page,uint8_t Coul,char *str,uint8_t width);
#endif