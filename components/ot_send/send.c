#include "ot_send.h"

#include <openthread/ping_sender.h>

/**
 * @file
 *  This file defines the functions used for the sender to send
 *  packets to a given destination, at repeated intervals.
*/

/**
 *
*/
void setSourceMleid(otInstance *aInstance, otPingSenderConfig *aConfig) {
  const otIp6Address *mleid = NULL;
  while (mleid == NULL) {
    mleid = otThreadGetMeshLocalEid(aInstance);
    vTaskDelay(DEFAULT_WAIT_TIME);
  }

  aConfig->mSource = *mleid;
  return;
}

#define MLEID "fdc2:53d3:bb7e:2437:114d:c3da:9918:d5e4"

void ping(otInstance *aInstance) {
  otPingSenderConfig aConfig;

  setSourceMleid(aInstance, &(aConfig));

  otNetifAddress netifDst;
  otIp6AddressFromString(MLEID, &(netifDst.mAddress));

#if DEBUG
  char* aBuffer = calloc(1, OT_IP6_ADDRESS_STRING_SIZE);
  otIp6AddressToString(&(netifDst.mAddress), aBuffer, OT_IP6_ADDRESS_STRING_SIZE);

  DEBUG_PRINT(
    otLogNotePlat("The destination address is %s", aBuffer)
  );

  free(aBuffer);
#endif // DEBUG

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

  PRINT_ERROR(otPingSenderPing(aInstance, &aConfig));

#if DEBUG
  DEBUG_PRINT(
    otLogNotePlat("Sent ping!")
  );
#endif // DEBUG
  return;
};