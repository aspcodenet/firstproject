#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_random.h"

// https://wokwi.com/projects/378092099167418369



#define SEND_BUTTON_PIN 25
#define RECEIVE_BUTTON_PIN 26

QueueHandle_t  queue=NULL;

int getTemperature(){
    ESP_LOGI("SEND", "Getting temp from sensor");
    return esp_random() % 30;
}

void receiveTask(){
    gpio_reset_pin(RECEIVE_BUTTON_PIN);
    gpio_set_pull_mode(RECEIVE_BUTTON_PIN, GPIO_PULLUP_ONLY);
    int currentValue = gpio_get_level(RECEIVE_BUTTON_PIN);
    int lastValue = 0;
    int temp;
     while(1){
        
        lastValue = currentValue;
        //Check button
        currentValue = gpio_get_level(RECEIVE_BUTTON_PIN);
        //ESP_LOGE("main","Press %d", currentValue);
        if(lastValue == 1 && currentValue == 0){
            vTaskDelay(20 / portTICK_PERIOD_MS);
            currentValue = gpio_get_level(RECEIVE_BUTTON_PIN);
            if(currentValue == 0){
                if(xQueueReceive(queue,&temp,(TickType_t )(1000/portTICK_PERIOD_MS)) == pdTRUE){
                    ESP_LOGE("RECEIVE","Press %d", temp);
                }
            }
        }
     }
}



void sendTask(){
    gpio_reset_pin(SEND_BUTTON_PIN);
    gpio_set_pull_mode(SEND_BUTTON_PIN, GPIO_PULLUP_ONLY);
    int currentValue = gpio_get_level(SEND_BUTTON_PIN);
    int lastValue = 0;

     while(1){
        lastValue = currentValue;
        //Check button
        currentValue = gpio_get_level(SEND_BUTTON_PIN);
        //ESP_LOGE("main","Press %d", currentValue);
        if(lastValue == 1 && currentValue == 0){
            vTaskDelay(20 / portTICK_PERIOD_MS);
            currentValue = gpio_get_level(SEND_BUTTON_PIN);
            if(currentValue == 0){
                //Get value
                int temp = getTemperature();
                ESP_LOGE("SEND","Press %d",  temp);
                xQueueSend(queue,(void *)&temp,0); // a
            }
        }
     }
}





void app_main(void)
{
    queue=xQueueCreate(20,sizeof(unsigned int));
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
    xTaskCreatePinnedToCore (
        receiveTask,     // Function to implement the task
        "receiveTask",   // Name of the task
        4096,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        
    xTaskCreatePinnedToCore (
        sendTask,     // Function to implement the task
        "sendTask",   // Name of the task
        4096,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        

    while(1){
        
        vTaskDelay(400 / portTICK_PERIOD_MS);
    }


}
