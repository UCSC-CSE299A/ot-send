#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

#include "ot_send.h"
#include "led.h"

#define MAX_CHARS 40

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

uint16_t udpAttachPayload(otMessage *aMessage) {
  static uint16_t count = 0;

  char payload[MAX_CHARS];
  sprintf(payload, "Packet number %" PRIu16 "", count);
  count += 1;

  otError error = otMessageAppend(aMessage, payload, MAX_CHARS * sizeof(char));
  handleMessageError(aMessage, error);

  return count - 1;
}

void udpSend(otInstance *aInstance,
             uint16_t port,
             uint16_t destPort,
             otUdpSocket *aSocket,
             otMessageInfo *aMessageInfo)
{
  otMessage *aMessage = otUdpNewMessage(aInstance, NULL);

  int count = udpAttachPayload(aMessage);
  otLogNotePlat("Sent UDP packet %d", count);

  otError error = otUdpSend(aInstance, aSocket, aMessage, aMessageInfo);
  handleMessageError(aMessage, error);
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
  aMessageInfo.mHopLimit = 0;  // default
  otIp6Address *peerAddr = &(aMessageInfo.mPeerAddr);
  handleError(otIp6AddressFromString(MLEID_MULTICAST, peerAddr));

  while (true) {
    setLed(ON);
    vTaskDelay(SEND_WAIT_TIME);
    udpSend(aInstance, port, destPort, aSocket, &aMessageInfo);

    setLed(OFF);
    vTaskDelay(PACKET_SEND_DELAY);
  }
  return;
}