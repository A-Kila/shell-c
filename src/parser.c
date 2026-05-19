#include "parser.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *replace_home(const char *token) {
    const char *home = getenv("HOME");
    if (!home) return NULL;

    size_t slen = strlen(home) + strlen(token); // no + 1 neccessry, token has extra ~
    char *replaced = malloc(slen);
    snprintf(replaced, slen, "%s%s", home, token + 1);

    return replaced;
}

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    for (char *token = strtok(input, " "); token != NULL; token = strtok(NULL, " ")) {
        if (command_out->argc >= MAX_ARGS - 1) break;

        char *argv = strdup(token);
        if (token[0] == '~') {
            char *tmp = replace_home(token);
            if (tmp) {
                free(argv);
                argv = tmp;
            }
        }

        command_out->argv[command_out->argc++] = argv;
    }

    command_out->argv[command_out->argc] = NULL;
    return command_out->argc > 0;
}

void command_free(command_t *command) {
    for (size_t i = 0; i < command->argc; i++) {
        free(command->argv[i]);
    }
}
