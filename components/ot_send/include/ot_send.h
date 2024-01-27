#ifndef OT_SEND_
#define OT_SEND_

#include "esp_ot_config.h"
#include "ot_send.h"

#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_openthread.h"
#include "esp_openthread_cli.h"
#include "esp_openthread_lock.h"
#include "esp_openthread_netif_glue.h"
#include "esp_openthread_types.h"
#include "esp_vfs_eventfd.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/uart_types.h"
#include "nvs_flash.h"
#include "openthread/cli.h"
#include "openthread/instance.h"
#include "openthread/logging.h"
#include "openthread/tasklet.h"

#define DEBUG true
#define DELIMITER "************************************"
#define PRINT_DELIMIER otLogNotePlat(DELIMITER)
#define DEBUG_PRINT(ot_note) PRINT_DELIMIER; ot_note; PRINT_DELIMIER;
#define PRINT_ERROR(ot_error) DEBUG_PRINT(otThreadErrorToString(ot_error))

#define MS_TO_TICKS(ms) ms / portTICK_PERIOD_MS
#define DEFAULT_WAIT_TIME MS_TO_TICKS(100)
#define PACKET_SEND_DELAY MS_TO_TICKS(5000) // 5 seconds

#define OT_DISCONNECTED(role) (role == OT_DEVICE_ROLE_DISABLED) || (role == OT_DEVICE_ROLE_DETACHED)

/**
 * https://openthread.io/guides/thread-primer/ipv6-addressing#multicast
*/
#define MLEID_MULTICAST "ff03::1"

/**
 * Creates a worker thread running OpenThread.
 *
 * @param[in]
 *  A pointer to the application layer context `aContext`.
*/
void ot_task_worker(void *aContext);

/**
 * Waits until the device is successfully connected into the Thread
 * WLAN before calling `ping()` in an infinite loop.
 *
 * @param[in]
 *  A pointer to the currrent OpenThread instance `aInstance`.
 *
 * @param[in]
 *  Sleep for `delay` ticks before sending another ping packet.
*/
void start_ping(otInstance *aInstance, const TickType_t delay);

/**
 * Sends ICMP "ping" packets to the Thread border router.
 *
 * @param[in]
 *  A pointer to the current OpenThread instance `aInstance`.
*/
void ping(otInstance *aInstance);

/**
 * Will wait until the device has been connected to the Thread network
 * before running
 *
 * @param[in]
 *  A pointer to the current OpenThread instance `aInstance`.
*/
void checkConnection(otInstance *aInstance);

#endif // ESP_OT_SEND_
