#include <stdio.h>

#include "ot_send.h"

#define MAX_CHARS 22

otError handleError(otError error) {
  if (error != OT_ERROR_NONE) {
    ERROR_PRINT(otLogCritPlat("%s", otThreadErrorToString(error)));
  }
  return error;
}

void handleMessageError(otMessage *aMessage, otError error) {
  if (handleError(error) != OT_ERROR_NONE) {
    otMessageFree(aMessage);
  }
  return;
}

otUdpSocket *udpCreateSocket(otInstance *aInstance, otSockAddr *aSockName) {
  otUdpSocket *aSocket = calloc(1, sizeof(otUdpSocket));
  handleError(otUdpOpen(aInstance, aSocket, NULL, NULL));

  handleError(otUdpBind(aInstance, aSocket, aSockName, OT_NETIF_THREAD));
  return aSocket;
}

int udpAttachPayload(otMessage *aMessage) {
  static int count = 0;

  char* payload = calloc(MAX_CHARS, sizeof(char));
  sprintf(payload, "Packet Number %d", count);
  count += 1;

  otError error = otMessageAppend(aMessage, payload, MAX_CHARS * sizeof(char));
  handleMessageError(aMessage, error);

  free(payload);
  return count - 1;
}

void udpSend(otInstance *aInstance,
             uint16_t port,
             uint16_t destPort,
             otUdpSocket *aSocket,
             otMessageInfo *aMessageInfo) {
  otMessage *aMessage = otUdpNewMessage(aInstance, NULL);

  int count = udpAttachPayload(aMessage);
  otLogNotePlat("Sent UDP packet %d", count);

  otError error = otUdpSend(aInstance, aSocket, aMessage, aMessageInfo);
  handleMessageError(aMessage, error);

  vTaskDelay(PACKET_SEND_DELAY);
  return;
}

void udpSendInfinite(otInstance *aInstance, uint16_t port, uint16_t destPort) {
  checkConnection(aInstance);

  otSockAddr aSockName;
  aSockName.mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName.mPort = port;
  otUdpSocket *aSocket = udpCreateSocket(aInstance, &aSockName);

  otMessageInfo aMessageInfo;
  aMessageInfo.mSockAddr = *otThreadGetMeshLocalEid(aInstance);
  aMessageInfo.mSockPort = port;
  aMessageInfo.mPeerPort = destPort;
  aMessageInfo.mLinkInfo = NULL;
  aMessageInfo.mHopLimit = 0;  // default
  otIp6Address *peerAddr = &(aMessageInfo.mPeerAddr);
  handleError(otIp6AddressFromString(MLEID_MULTICAST, peerAddr));

  while (true) {
    udpSend(aInstance, port, destPort, aSocket, &aMessageInfo);
  }
  return;
}