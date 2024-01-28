#include "cli_text.h"

/**
 * @file
 * Print to the CLI interface a set of strings.
 *
 * @cli cse299a
 * @code
 * cse299a
 * Hello World from the OpenThread CLI!
 * The changes from OpenThread-CSE299A have been added.
 * @endcode
 * @par    
 *
 * Based upon the example code given by OpenThread on how
 * to create a custom CLI command:
 * 
 * https://github.com/openthread/openthread/blob/main/src/cli/cli_extension_example.c#L56
*/

otError cse299aCommand(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error = OT_ERROR_NONE;
    
    if (aArgsLength > 0) {
        OutputLine("Hello World from the OpenThread CLI!");
        OutputLine("The changes from OpenThread-CSE299A have been added.");
    }
    else {
        error = OT_ERROR_INVALID_COMMAND;
    }

    return error;
}
