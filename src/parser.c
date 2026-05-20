#include "parser.h"
#include "shell.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *replace_head(const char *token) {
    const char *home = getenv("HOME");
    if (!home) return NULL;

    size_t len = strlen(home) + strlen(token); // no +1 necessary, token has extra ~
    char *replaced = malloc(len);
    snprintf(replaced, len, "%s%s", home, token + 1);

    return replaced;
}

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    char buf[BUFFER_SIZE];
    size_t len = 0;
    bool inside_sq = false;

    for (char *ch = input; ; ch++) {
        if (*ch == '\'') {
            inside_sq = !inside_sq;
            continue;
        }

        if (*ch == '\0' || (*ch == ' ' && !inside_sq)) {
            if (len > 0) {
                char *arg = (buf[0] == '~') ? replace_head(buf) : NULL;

                if (!arg) arg = strdup(buf);
                command_out->argv[command_out->argc++] = arg;
 
                buf[len] = '\0';
                len = 0;
            }

            if (*ch == '\0') break;

            continue;
        }
 
        buf[len++] = *ch;
    }

    command_out->argv[command_out->argc] = NULL;
    return command_out->argc > 0;
}

void command_free(command_t *command) {
    for (size_t i = 0; i < command->argc; i++) {
        free(command->argv[i]);
    }
}
