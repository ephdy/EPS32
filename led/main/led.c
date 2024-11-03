#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

static EventGroupHandle_t ledc_event_handle;

bool IRAM_ATTR ledc_finish_cb(const ledc_cb_param_t *param,void *user_arg){
    BaseType_t taskWoken;
    if(param->duty){
        xEventGroupSetBitsFromISR(ledc_event_handle,BIT0,&taskWoken);
    }
    else{
        xEventGroupSetBitsFromISR(ledc_event_handle,BIT1,&taskWoken);
    }
    return taskWoken;
}


void led_run_task(void* param){
    //char gpio_level=0;
    EventBits_t ev;
    while (1)
    {
        /*
        gpio_level=gpio_level?0:1;
        gpio_set_level(GPIO_NUM_2,gpio_level);
        vTaskDelay(pdMS_TO_TICKS(500));
        */
        ev =xEventGroupWaitBits(ledc_event_handle,BIT0|BIT1,pdTRUE,pdFALSE,pdMS_TO_TICKS(50000));
        if (ev&BIT0)
        {
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,5000);
            ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
        }
        if(ev&BIT1)
        {
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8191,5000);
            ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
        }
        ledc_cbs_t cbs={
        .fade_cb=ledc_finish_cb,
         };
        ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);
    }
    
}


void app_main(void)
{
    gpio_config_t led_cfg={
        .pin_bit_mask=(1<<GPIO_NUM_2),
        .pull_up_en=GPIO_PULLDOWN_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .intr_type=GPIO_INTR_DISABLE,
    };
    gpio_config(&led_cfg);

    ledc_timer_config_t ledc_timer={
        .speed_mode=LEDC_LOW_SPEED_MODE,
        .timer_num=LEDC_TIMER_0,
        .clk_cfg=LEDC_AUTO_CLK,
        .duty_resolution=LEDC_TIMER_13_BIT,
        .freq_hz=5000,
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel={
        .speed_mode=LEDC_LOW_SPEED_MODE,
        .gpio_num=GPIO_NUM_2,
        .intr_type=LEDC_INTR_DISABLE,
        .duty=0,
        .timer_sel=LEDC_TIMER_0,
        .channel=LEDC_CHANNEL_0,
    };
    ledc_channel_config(&ledc_channel);

    ledc_fade_func_install(0);
    ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8191,5000);
    ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);

    ledc_cbs_t cbs={
        .fade_cb=ledc_finish_cb,
    };
    ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);


    xTaskCreatePinnedToCore(led_run_task,"led",2048,NULL,3,NULL,1);
    
    
}
