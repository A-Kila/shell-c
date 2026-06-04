#include "parser.h"
#include "shell.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NULL_CHAR '\0'
#define SPACE ' '
#define TILDE '~'
#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '"'
#define ESCAPE '\\'
#define DOLLAR '$'

static char *replace_head(const char *token) {
    const char *home = getenv("HOME");
    if (!home || token[0] != TILDE) return NULL;

    size_t len = strlen(home) + strlen(token); // no +1 necessary, token has extra ~
    char *replaced = malloc(len);
    snprintf(replaced, len, "%s%s", home, token + 1);

    return replaced;
}

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    char buf[BUFFER_SIZE];
    size_t len = 0;
    bool inside_sq = false, inside_dq = false;
    bool special = false;

    for (char *ch = input; len < BUFFER_SIZE && command_out->argc < MAX_ARGS; ch++) {
        if (special) {
            special = false;
            goto buf_add;
        }

        if (!inside_dq && *ch == SINGLE_QUOTE) {
            inside_sq = !inside_sq;
            continue;
        }

        if (!inside_sq && *ch == DOUBLE_QUOTE) {
            inside_dq = !inside_dq;
            continue;
        }

        if (!inside_sq && *ch == ESCAPE) {
            special = true;
            continue;
        }

        if (*ch == NULL_CHAR || (*ch == SPACE && !inside_sq && !inside_dq)) {
            if (len > 0) {
                buf[len] = NULL_CHAR;
                char *arg = (buf[0] == TILDE) ? replace_head(buf) : NULL;

                if (!arg) arg = strdup(buf);
                command_out->argv[command_out->argc++] = arg;
 
                len = 0;
            }

            if (*ch == NULL_CHAR) break;

            continue;
        }

buf_add:
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
