#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "connect_wifi.h"

static const char *TAG = "mqttwss_example";
#define CONFIG_BROKER_URI "mqtt://test.mosquitto.org"

//#define CONFIG_BROKER_URI "mqtt://stefan.holmberg:testfeed@io.adafruit.com"
#define TOPIC "stefanholmberg/feeds/testfeed"


int getTemperature(){
    ESP_LOGI("SEND", "Getting temp from sensor");
    return esp_random() % 30;
}
#define SEND_BUTTON_PIN 25
#define RECEIVE_BUTTON_PIN 26

esp_mqtt_client_handle_t client;

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
                char text[20];
                sprintf(text,"apa %d",temp);
                int id = esp_mqtt_client_publish(client, TOPIC, text  , 0, 1, 0);
                ESP_LOGE("SEND","ID %d",  id);
            }
        }
     }
}


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    /* The argument passed to esp_mqtt_client_register_event can de accessed as handler_args*/
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URI,
        //  .credentials.username="stefanholmberg",
        //  .credentials.authentication.password="testfeed"
        //.broker.verification.certificate = (const char *)mqtt_eclipseprojects_io_pem_start,
    };

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

    esp_mqtt_client_start(client);
}

void app_main(void)
{
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	connect_wifi();

    esp_event_loop_create_default();

    xTaskCreatePinnedToCore (
        sendTask,     // Function to implement the task
        "sendTask",   // Name of the task
        4096,      // Stack size in words
        NULL,      // Task input parameter
        0,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
    );        



    mqtt_app_start();
    while(1){
        vTaskDelay(10000);
    }
}