/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * OpenThread Command Line Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
*/
#include "ot_send.h"
#include "led.h"

#define STACK_DEPTH 10240
#define OT_WORKER_PRIORIY 5
#define LED_WORKER_PRIORITY 5

/**
 * A seperate worker thread will keep flashing the built-in LED
 * while OpenThread is running.
*/
void ledFlashWorker(void* param) {
  Led *led = (Led *) param;
  setLed(led, OFF);
  while (true) {
    flashLed(led);
  }
  return;
}

void app_main(void)
{
    // Used eventfds:
    // * netif
    // * ot task queue
    // * radio driver
    esp_vfs_eventfd_config_t eventfd_config = {
        .max_fds = 3,
    };

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_vfs_eventfd_register(&eventfd_config));

    Led led;

#if CONFIG_LED_ENABLED
    initLed(&led);
    configureLed(&led);
    xTaskCreate(ledFlashWorker, "led_flash_worker", STACK_DEPTH,
                (void *) &led, LED_WORKER_PRIORITY, NULL);
#endif // CONFIG_LED_ENABLED

    xTaskCreate(ot_task_worker, "ot_cli_main", STACK_DEPTH,
                xTaskGetCurrentTaskHandle(), OT_WORKER_PRIORIY, NULL);

    udpSendInfinite(esp_openthread_get_instance(),
                    UDP_SOCK_PORT, UDP_DEST_PORT, &led);

#if CONFIG_LED_ENABLED
    freeLed(&led);
#endif
    return;
}