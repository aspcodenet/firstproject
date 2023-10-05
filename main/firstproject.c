#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
// https://wokwi.com/projects/377761117596697601



#define LED1_PIN 2
#define LED2_PIN 4
#define LED3_PIN 13

void app_main(void)
{
    gpio_reset_pin(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED3_PIN);
    gpio_set_direction(LED3_PIN, GPIO_MODE_OUTPUT);

    while(1){
        gpio_set_level(LED1_PIN,1 );
        gpio_set_level(LED2_PIN,1 );
        gpio_set_level(LED3_PIN,1 );
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED1_PIN,0 );
        gpio_set_level(LED2_PIN,0 );
        gpio_set_level(LED3_PIN,0 );
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
