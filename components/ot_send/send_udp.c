#include <stdio.h>

#include "ot_send.h"

#define MAX_CHARS 22

otUdpSocket *udpCreateSocket(otInstance *aInstance,
                             uint16_t port,
                             otSockAddr *aSockName) {
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
  static int count = 0;

  char* payload = calloc(MAX_CHARS, sizeof(char));
  sprintf(payload, "Packet Number %d", count);
  count += 1;

  otError error = otMessageAppend(aMessage, payload, MAX_CHARS * sizeof(char));
  free(payload);
  return error;
}

void udpSend(otInstance *aInstance,
             uint16_t port,
             uint16_t destPort,
             otUdpSocket *aSocket,
             otMessageInfo *aMessageInfo) {
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