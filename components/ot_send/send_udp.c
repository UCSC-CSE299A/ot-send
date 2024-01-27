#include "ot_send.h"

#include <openthread/udp.h>

/**
 * @file
 *  This file defines the functions used for the sender to send
 *  UDP packets to all devices in the Thread WLAN, at a specified interval.
*/

#define PORT 12345

/**
 *
*/
void createUdpSocket(otInstance *aInstance) {
  otUdpSocket aSocket;
  ESP_ERROR_CHECK(otUdpOpen(aInstance, &aSocket, NULL, NULL));

  otSockAddr aSockName;
  aSockName.mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName.mPort = PORT;

  ESP_ERROR_CHECK(otUdpBind(aInstance, &aSocket, &aSockName, OT_NETIF_THREAD));
  return;
}