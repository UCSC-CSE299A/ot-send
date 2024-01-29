#include "ot_send.h"

#include <openthread/ping_sender.h>

/**
 * @file
 *  This file defines the functions used for the sender to send
 *  ICMP packets to all devices in the Thread WLAN, at a specified interval.
*/

otError startPing(otInstance *aInstance, const TickType_t delay) {
  checkConnection(aInstance);
  while (true) {
    if (esp_openthread_lock_acquire(DEFAULT_WAIT_TIME)) {
      otError error = ping(aInstance);

      if (error != OT_ERROR_NONE) {
        ERROR_PRINT(otLogCritPlat("%s", otThreadErrorToString(error)));
        ERROR_PRINT(otLogCritPlat("Ending infinite ping."));
        return error;
      } else {
        DEBUG_PRINT(otLogNotePlat("Successfully sent ping packets."));
      }
      esp_openthread_lock_release();      
    }

    DEBUG_PRINT(otLogNotePlat("Waiting for the OpenThread lock."));
    vTaskDelay(delay);
  }
  return OT_ERROR_NONE;
}

otError ping(otInstance *aInstance) {
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

  return otPingSenderPing(aInstance, &aConfig);
};