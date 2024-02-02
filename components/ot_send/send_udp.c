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

otUdpSocket *udpCreateSocket(otInstance *aInstance,
                             uint16_t port,
                             otSockAddr *aSockName) {
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

  handleError(otMessageAppend(aMessage, payload, MAX_CHARS * sizeof(char)));
  free(payload);
  return count;
}

void udpSend(otInstance *aInstance,
             uint16_t port,
             uint16_t destPort,
             otUdpSocket *aSocket,
             otMessageInfo *aMessageInfo) {
  otMessage *aMessage = otUdpNewMessage(aInstance, NULL);

  otError error = udpAttachPayload(aMessage);
  handleMessageError(aMessage, error);

  int count = otUdpSend(aInstance, aSocket, aMessage, aMessageInfo);
  otLogNotePlat("Sent UDP packet %d", count);

  vTaskDelay(PACKET_SEND_DELAY);
  return;
}

void udpSendInfinite(otInstance *aInstance, uint16_t port, uint16_t destPort) {
  checkConnection(aInstance);

  otSockAddr aSockName;
  aSockName.mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName.mPort = port;
  otUdpSocket *aSocket = udpCreateSocket(aInstance, port, &aSockName);

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