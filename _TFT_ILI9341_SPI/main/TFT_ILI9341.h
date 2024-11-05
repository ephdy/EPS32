#ifndef TFT_ILI9341_H
#define TFT_ILI9341_H

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
#define PIN_NUM_BLK  19
#define RES_ON       gpio_set_level(GPIO_NUM_4,0)
#define RES_OFF      gpio_set_level(GPIO_NUM_4,1)
#define DC_Command   gpio_set_level(PIN_NUM_DC,0)
#define DC_Data      gpio_set_level(PIN_NUM_DC,1)   
#define BLK_ON       gpio_set_level(PIN_NUM_BLK,1)
#define BLK_OFF      gpio_set_level(PIN_NUM_BLK,0)

#define TFT_HOST    SPI3_HOST

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);
void spi_init();
void tft_init();
void tft_reset();
void tft_write_byte(uint8_t chr);
void tft_write_bytes(uint8_t chr[],uint8_t n);
void tft_write_cmd(uint8_t cmd);
void tft_write_data(uint8_t data);
void tft_write_data_16Bit(uint16_t data);
void tft_set_address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void tft_fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void tft_point(uint16_t x,uint16_t y,uint16_t color);


#endif