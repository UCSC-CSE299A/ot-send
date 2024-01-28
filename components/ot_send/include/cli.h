#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>
#include <stdbool.h>
#include <openthread/cli.h>

#include "ot_send.h"

#define OT_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define OT_SEND_CLI_USE true

void otCliSentCustomCommands(otInstance *aInstance);
otError cliIntroString(void *aContext, uint8_t aArgsLength, char *aArgs[]);
otError infinitePing(void *aContext, uint8_t aArgsLength, char *aArgs[]);

static const otCliCommand customCommands[] = {
    {"udp-cse299a", cliIntroString},
    {"infinite-ping", infinitePing},
};

#endif // CLI_H_