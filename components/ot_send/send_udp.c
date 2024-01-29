#include "ot_send.h"

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
otUdpSocket *udpCreateSocket(
  otInstance *aInstance,
  uint16_t port,
  otSockAddr *aSockName
) {
  otUdpSocket *aSocket = calloc(1, sizeof(otUdpSocket));
  ESP_ERROR_CHECK(otUdpOpen(aInstance, aSocket, NULL, NULL));

  ESP_ERROR_CHECK(otUdpBind(aInstance, aSocket, aSockName, OT_NETIF_THREAD));
  return aSocket;
}

void handleError(otMessage *aMessage, otError error) {
  ERROR_PRINT(otLogCritPlat("%s", otThreadErrorToString(error)));
  otMessageFree(aMessage);
  return;
}

otError udpAttachPayload(otMessage *aMessage) {
  char* payload = "Hello World! This is the UDP packet payload.";
  return otMessageAppend(aMessage, payload, sizeof(payload));
}

void udpSend(
  otInstance *aInstance,
  uint16_t port,
  uint16_t destPort,
  otUdpSocket *aSocket,
  otMessageInfo *aMessageInfo
) {
  otMessage *aMessage = otUdpNewMessage(aInstance, NULL);

  otError error = udpAttachPayload(aMessage);
  if (error != OT_ERROR_NONE) {
    handleError(aMessage, error);
  }

  error = otUdpSend(aInstance, aSocket, aMessage, aMessageInfo);
  if (error != OT_ERROR_NONE) {
    handleError(aMessage, error);
  }

  DEBUG_PRINT(otLogNotePlat("UDP packet successfully sent."));
  vTaskDelay(PACKET_SEND_DELAY);
  return;
}

void udpSendInfinite(otInstance *aInstance, uint16_t port, uint16_t destPort) {
  otSockAddr aSockName;
  aSockName.mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName.mPort = port;

  otUdpSocket *aSocket = udpCreateSocket(aInstance, port, &aSockName);
  checkConnection(aInstance);

  otMessageInfo aMessageInfo;
  aMessageInfo.mSockAddr = *otThreadGetMeshLocalEid(aInstance);
  aMessageInfo.mSockPort = port;
  aMessageInfo.mPeerPort = destPort;
  aMessageInfo.mLinkInfo = NULL;
  aMessageInfo.mHopLimit = 0;  // default
  otIp6Address *peerAddr = &(aMessageInfo.mPeerAddr);
  otIp6AddressFromString(MLEID_MULTICAST, peerAddr);

  while (true) {
    udpSend(aInstance, port, destPort, aSocket, &aMessageInfo);
  }
  return;
}