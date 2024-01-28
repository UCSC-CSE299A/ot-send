#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>
#include <openthread/cli.h>

#define OT_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define OT_SEND_CLI_USE 1

void otCliSentCustomCommands(otInstance *aInstance);
otError cliIntroString(void *aContext, uint8_t aArgsLength, char *aArgs[]);

static const otCliCommand customCommands[] = {
    {"udpcse299a", cliIntroString}
};

#endif // CLI_H_