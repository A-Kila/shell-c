#include "parser.h"
#include "shell.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *apply_tilde(const char *token) {
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

    for (char *p = input; ; p++) {
        if (*p == '\'') {
            inside_sq = !inside_sq;
        } else if (*p == '\0' || (*p == ' ' && !inside_sq)) {
            if (len > 0) {
                buf[len] = '\0';
                char *arg = (buf[0] == '~') ? apply_tilde(buf) : NULL;
                if (!arg) arg = strdup(buf);
                command_out->argv[command_out->argc++] = arg;
                len = 0;
            }
            if (*p == '\0') break;
        } else {
            buf[len++] = *p;
        }
    }

    command_out->argv[command_out->argc] = NULL;
    return command_out->argc > 0;
}

void command_free(command_t *command) {
    for (size_t i = 0; i < command->argc; i++) {
        free(command->argv[i]);
    }
}
