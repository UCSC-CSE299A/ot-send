#include "ot_send.h"

#include <openthread/udp.h>

/**
 * @todo
 *  Determine what you need to free, so you can then create and free UDP packets,
 *  rather than repeatedly sending the same UDP packet instance.
*/

/**
 * Creates a UDP socket at the specified port.
 *
 * @param[in]
 *  The OpenThread instance `aInstance`.
 *
 * @param[in]
 *  The `port` to use in the UDP socket.
*/
otUdpSocket *udpCreateSocket(otInstance *aInstance, uint16_t port) {
  otUdpSocket *aSocket = calloc(1, sizeof(otUdpSocket));
  ESP_ERROR_CHECK(otUdpOpen(aInstance, aSocket, NULL, NULL));

  /**
   * @todo
   *  Free the heap data associated with `aSockName`.
  */
  otSockAddr *aSockName = calloc(1, sizeof(otSockAddr));
  aSockName->mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName->mPort = port;

  ESP_ERROR_CHECK(otUdpBind(aInstance, aSocket, aSockName, OT_NETIF_THREAD));
  return aSocket;
}

/**
 *
*/
void udpSend(otInstance *aInstance, uint16_t port, uint16_t destPort) {
  checkConnection(aInstance);

  otUdpSocket *aSocket = udpCreateSocket(aInstance, port);
  otMessage *aMessage = otUdpNewMessage(aInstance, NULL);

  /**
   * @todo
   *  Free the heap data associated with `aSockName`.
  */
  otMessageInfo *aMessageInfo = calloc(1, sizeof(otMessageInfo));
  aMessageInfo->mSockAddr = *otThreadGetMeshLocalEid(aInstance);
  aMessageInfo->mSockPort = port;
  aMessageInfo->mPeerPort = destPort;
  aMessageInfo->mLinkInfo = NULL;
  aMessageInfo->mHopLimit = 0;  // default
  otIp6Address *peerAddr = &(aMessageInfo->mPeerAddr);
  otIp6AddressFromString(MLEID_MULTICAST, peerAddr);

  // ESP_ERROR_CHECK(otUdpSend(aInstance, aSocket, aMessage, &aMessageInfo));
  while (true) {
    otUdpSend(aInstance, aSocket, aMessage, aMessageInfo);
  }
  return;
}