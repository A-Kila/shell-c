#include "parser.h"
#include "shell.h"
#include <_string.h>
#include <_strings.h>
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

static char *strcat_heap(const char *str1, const char *str2, bool is_space) {
    char *res = malloc(strlen(str1) + strlen(str2) + (is_space ? 2 : 1));
    strcat(res, str1);
    if (is_space) strcat(res, " ");
    strcat(res, str2);

    return res;
}

static char *handle_sqoutes(const char *token, char **rest) {
    if (!*rest) *rest = "";

    char *full_string = strcat_heap(token, *rest, true);

    char *res = NULL;
    size_t res_len = 0;
    for (token = strtok_r(full_string, "'", rest); token; token = strtok_r(*rest, "'", rest)) {
        if (!res) {
            res = strdup(token);
            continue;
        }
        char *tmp = res;
        res = strcat_heap(res, token, false);
        free(tmp);

        if (*rest && *rest[0] == ' ') break;
    }

    return res;
}

bool parse(command_t *command_out, char *input) {
    command_out->argc = 0;

    char *rest;
    for (char *token = strtok_r(input, " ", &rest); token != NULL;) {
        if (command_out->argc >= MAX_ARGS - 1) break;
 
        char *argv = NULL;

        if (index(token, '\'')) argv = handle_sqoutes(token, &rest);
        if (token[0] == '~') argv = replace_home(token);

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
