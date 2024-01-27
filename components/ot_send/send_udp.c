#include "ot_send.h"

#include <openthread/udp.h>

/**
 * Creates a UDP socket at the specified port.
 *
 * @param[in]
 *  The OpenThread instance `aInstance`.
 *
 * @param[in]
 *  The `port` to use in the UDP socket.
*/
void createUdpSocket(otInstance *aInstance, uint16_t port) {
  otUdpSocket aSocket;
  ESP_ERROR_CHECK(otUdpOpen(aInstance, &aSocket, NULL, NULL));

  otSockAddr aSockName;
  aSockName.mAddress = *otThreadGetMeshLocalEid(aInstance);
  aSockName.mPort = port;

  ESP_ERROR_CHECK(otUdpBind(aInstance, &aSocket, &aSockName, OT_NETIF_THREAD));
  return;
}

/**
 *
*/
void udpSend(otInstance *aInstance, uint16_t port) {
  checkConnection(aInstance);
  createUdpSocket(aInstance, port);
  return;
}