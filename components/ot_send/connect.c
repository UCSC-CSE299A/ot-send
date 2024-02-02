#include "ot_send.h"
#include <openthread/thread.h>

#define NETWORK_KEY "52e83530bb0cebcdbe7e12e6d878b8c6"

void checkConnection(otInstance *aInstance) {
  otDeviceRole currentRole;
  do {
    currentRole = otThreadGetDeviceRole(aInstance);
    vTaskDelay(DEFAULT_WAIT_TIME);
  }
  while(OT_DISCONNECTED(currentRole));

  DEBUG_PRINT(otLogNotePlat("OpenThread Connection has been established."));
  return;
}