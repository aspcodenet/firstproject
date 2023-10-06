#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
// https://wokwi.com/projects/377761117596697601



#define LED1_PIN 2
#define LED2_PIN 4
#define LED3_PIN 13

void led1funktionen(){
    while(1){
        gpio_set_level(LED1_PIN,1 );
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED1_PIN,0 );
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void led2funktionen(){
    while(1){
        gpio_set_level(LED2_PIN,1 );
        vTaskDelay(700 / portTICK_PERIOD_MS);
        gpio_set_level(LED2_PIN,0 );
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void led3funktionen(){
    while(1){
        gpio_set_level(LED3_PIN,1 );
        vTaskDelay(1500 / portTICK_PERIOD_MS);
        gpio_set_level(LED3_PIN,0 );
        vTaskDelay(400 / portTICK_PERIOD_MS);
    }
}



void app_main(void)
{
    gpio_reset_pin(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED3_PIN);
    gpio_set_direction(LED3_PIN, GPIO_MODE_OUTPUT);

    xTaskCreatePinnedToCore (
        led1funktionen,     // Function to implement the task
        "led1task",   // Name of the task
        1000,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        
    xTaskCreatePinnedToCore (
        led2funktionen,     // Function to implement the task
        "led2task",   // Name of the task
        1000,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        
    xTaskCreatePinnedToCore (
        led3funktionen,     // Function to implement the task
        "led3task",   // Name of the task
        1000,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        


    while(1){
        // OBS STEFAN SLÖSAR PP DENNA TASK        //
        // GÖR NÅT VETTIGT HÄR MED
         //stoppas in koden för en task
        // alt tex kolla om pressed button
        vTaskDelay(400 / portTICK_PERIOD_MS);

    }

}
