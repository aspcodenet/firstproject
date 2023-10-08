#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
#include "esp_log.h"

// https://wokwi.com/projects/378039853550046209



#define LED_PIN 26
#define BUTTON_PIN 25

// void led1funktionen(){
//     while(1){
//         gpio_set_level(LED1_PIN,1 );
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//         gpio_set_level(LED1_PIN,0 );
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }


// void led2funktionen(){
//     while(1){
//         gpio_set_level(LED2_PIN,1 );
//         vTaskDelay(700 / portTICK_PERIOD_MS);
//         gpio_set_level(LED2_PIN,0 );
//         vTaskDelay(200 / portTICK_PERIOD_MS);
//     }
// }



void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    //gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT_OUTPUT);
    //gpio_set_level(BUTTON_PIN,1 );

    // xTaskCreatePinnedToCore (
    //     led1funktionen,     // Function to implement the task
    //     "led1task",   // Name of the task
    //     1000,      // Stack size in words
    //     NULL,      // Task input parameter
    //     0,         // Priority of the task
    //     NULL,      // Task handle.
    //     1          // Core where the task should run
    // );        
    // xTaskCreatePinnedToCore (
    //     led2funktionen,     // Function to implement the task
    //     "led2task",   // Name of the task
    //     1000,      // Stack size in words
    //     NULL,      // Task input parameter
    //     0,         // Priority of the task
    //     NULL,      // Task handle.
    //     1          // Core where the task should run
    // );        
    // xTaskCreatePinnedToCore (
    //     led3funktionen,     // Function to implement the task
    //     "led3task",   // Name of the task
    //     1000,      // Stack size in words
    //     NULL,      // Task input parameter
    //     0,         // Priority of the task
    //     NULL,      // Task handle.
    //     1          // Core where the task should run
    // );        

    int currentValue = gpio_get_level(BUTTON_PIN);
    int lastValue = 0;
    int ledState = 0;
    while(1){
        lastValue = currentValue;
        //Check button
        currentValue = gpio_get_level(BUTTON_PIN);
        //ESP_LOGE("main","Press %d", currentValue);
        if(lastValue == 1 && currentValue == 0){
            vTaskDelay(20 / portTICK_PERIOD_MS);
            currentValue = gpio_get_level(BUTTON_PIN);
            if(currentValue == 0){
                ESP_LOGE("main","Press");
                ledState = !ledState;
                gpio_set_level(LED_PIN,ledState);
            }
        }
        //vTaskDelay(400 / portTICK_PERIOD_MS);

    }

}
