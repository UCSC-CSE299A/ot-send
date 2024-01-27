#include "ot_send.h"

#include <openthread/ping_sender.h>

/**
 * @file
 *  This file defines the functions used for the sender to send
 *  packets to a given destination, at repeated intervals.
*/

void start_ping(otInstance *aInstance, const TickType_t delay) {
  otDeviceRole currentRole;
  do {
    currentRole = otThreadGetDeviceRole(aInstance);
    vTaskDelay(DEFAULT_WAIT_TIME);
  } while(OT_DISCONNECTED(currentRole));

  while (true) {
    ping(aInstance);
    vTaskDelay(delay);
  }
  return;
}

void ping(otInstance *aInstance) {
  otPingSenderConfig aConfig;

  aConfig.mSource = *otThreadGetMeshLocalEid(aInstance);

  otIp6Address *destAddr = &(aConfig.mDestination);
  otIp6AddressFromString(MLEID_MULTICAST, destAddr);

  aConfig.mTimeout = 100; // ms
  aConfig.mMulticastLoop = true;

  // Default settings
  aConfig.mReplyCallback = NULL;
  aConfig.mStatisticsCallback = NULL;
  aConfig.mCallbackContext = NULL;
  aConfig.mSize = 0;
  aConfig.mCount = 0;
  aConfig.mInterval = 0;
  aConfig.mTimeout = 100; // ms
  aConfig.mHopLimit = 0;

  ESP_ERROR_CHECK(otPingSenderPing(aInstance, &aConfig));
  return;
};