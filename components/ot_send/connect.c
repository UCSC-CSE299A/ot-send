#include "ot_send.h"
#include <openthread/thread.h>

void checkConnection(otInstance *aInstance) {
  otDeviceRole currentRole;
  do {
    currentRole = otThreadGetDeviceRole(aInstance);
    vTaskDelay(DEFAULT_WAIT_TIME);
  }
  while(OT_DISCONNECTED(currentRole));

  otLogNotePlat("OpenThread Connection has been established.");
  return;
}