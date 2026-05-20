#include "parser.h"
#include "shell.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *replace_home(const char *token) {
    const char *home = getenv("HOME");
    if (!home) return NULL;

    size_t len = strlen(home) + strlen(token); // no + 1 neccessry, token has extra ~
    char *replaced = malloc(len);
    snprintf(replaced, len, "%s%s", home, token + 1);

    return replaced;
}

static char *handle_sqoutes(const char *token, char **rest) {
    char buf[BUFFER_SIZE] = {0};
    size_t len = 0;
    bool inside = false;

    for (const char *ch = token; *ch; ch++) {
        if (*ch == '\'') {
            inside = !inside; 
            continue; 
        }

        buf[len++] = *ch;
    }

    if (inside && *rest) {
        for (char *ch = *rest; true; ch++) {
            if (*ch == '\0') { 
                *rest = NULL; 
                break; 
            }
 
            if (*ch == '\'') { 
                inside = !inside; 
                continue; 
            }

            if (!inside && *ch == ' ') { 
                *rest = ch + 1; 
                break; 
            }

            buf[len++] = *ch;
        }
    }

    return strdup(buf);
}

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    char *rest;
    for (char *token = strtok_r(input, " ", &rest); token != NULL;) {
        if (command_out->argc >= MAX_ARGS - 1) break;
 
        char *argv = NULL;

        if (index(token, '\'')) argv = handle_sqoutes(token, &rest);
        else if (token[0] == '~') argv = replace_home(token);

        if (!argv) argv = strdup(token);
        command_out->argv[command_out->argc++] = argv;
 
        // printf("argv - %s;  rest - %s\n", argv, rest);
        token = strtok_r(rest, " ", &rest);
    }

    command_out->argv[command_out->argc] = NULL;
    return command_out->argc > 0;
}

void command_free(command_t *command) {
    for (size_t i = 0; i < command->argc; i++) {
        free(command->argv[i]);
    }
}
