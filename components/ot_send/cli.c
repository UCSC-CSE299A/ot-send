#include "cli.h"

/**
 * @file
 * Print to the CLI interface a set of strings. 
 * 
 * https://github.com/openthread/openthread/blob/main/src/cli/cli_extension_example.c#L56
 * https://github.com/UCSC-CSE299A/ot-send-receive/blob/master/managed_components/espressif__esp_ot_cli_extension/src/esp_ot_cli_extension.c#L50
*/

void otCliSentCustomCommands(otInstance *aInstance)
{
  otCliSetUserCommands(customCommands, OT_ARRAY_LENGTH(customCommands), aInstance);
  return;
}

/**
 * @cli cse299a
 * @code
 * cse299a
 * Hello World from CSE 299A!
 * If you see this message, this means you have access to
 * the CSE 299A UDP Sender.
 * @endcode
 * @par
*/
otError cliIntroString(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error = OT_ERROR_NONE;
    
    if (aArgsLength == 0) {
        otCliOutputFormat("Hello World from CSE 299A!\n");
        otCliOutputFormat("If you see this message, this means you have\n");
        otCliOutputFormat("access to the CSE 299A UDP Sender.\n");
    }
    else {
        error = OT_ERROR_INVALID_COMMAND;
    }

    return error;
}

otError infinitePing(void *aContext, uint8_t aArgsLength, char *aArgs[]) {
  return startPing(esp_openthread_get_instance(), PACKET_SEND_DELAY);
}

otError startUdpInfinite(void *aContext, uint8_t aArgsLength, char *aArgs[]) {
  udpSendInfinite(esp_openthread_get_instance(), UDP_SOCK_PORT, UDP_DEST_PORT);
  return OT_ERROR_NONE;
}