#include "parser.h"
#include <string.h>

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    for (char *token = strtok(input, " "); token != NULL; token = strtok(NULL, " ")) {
        if (command_out->argc >= MAX_ARGS - 1) break;
        command_out->argv[command_out->argc++] = token;
    }

    command_out->argv[command_out->argc] = NULL;
    return command_out->argc > 0;
}
