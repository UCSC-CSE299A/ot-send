#include "ot_send.h"

#include <openthread/ping_sender.h>

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

/**
 * @file
 *  This file defines the functions used for the sender to send
 *  packets to a given destination, at repeated intervals.
 *
 * @todo
 *  Do a simple ping to the border router to demonstrate that you can successfully
 *  send packets programmatically.
 *
 *  1. Get the source IP address. This will involve calling "otIp6GetUnicastAddresses()"
 *     and traversing to the linked list of "otIp6Address" objects until you find
 *     the multicast address.
 *
 *     A good idea is to look at the source code for the "ipaddr" command to see
 *     how this can be done.
 *
 *  2. Get the destination IP address.
 *
 *  3. Fill out the minimal configurations to get ping working.
 *
 *  4. Send it to the destination! Check Wireshark to make sure you have successfully
 *     sent ping packets at the app layer at deined intervals.
 *
 *     If need be, use the FreeRTOS API to sleep and then send packets at regular
 *     intervals (although you may not need to do this, as I believe OpenThread
 *     can already do this).
 *
 *  5. See if you can pack application layer data ontop of ICMP through ping.
 *     If so, all you need to do at the receiver side is to decrypt the app layer
 *     payload and print it out to the console!
*/

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

  otPingSenderPing(aInstance, &aConfig);

#if DEBUG
  DEBUG_PRINT(
    otLogNotePlat("Sent ping!")
  );
#endif // DEBUG
  return;
};