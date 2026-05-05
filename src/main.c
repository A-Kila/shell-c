#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SHELL_MAX_CHARS 256
#define NUM_BUILTINS 3

typedef bool (*builtin_fun_t)(char *);
typedef struct {
    char *name;
    bool needs_args;
    builtin_fun_t program;
} builtin_t;

bool my_exit(char *);
bool echo(char *);
bool type(char *);

const builtin_t builtins[NUM_BUILTINS] = {
    { "exit", false, my_exit },
    { "echo", true,  echo },
    { "type", true,  type }
};

bool my_exit(char *_) {
    return true;
}

bool echo(char *arguments) {
    printf("%s\n", arguments);
    return false;
}

bool type(char *arguments) {
    bool is_builtin = false;

    for (size_t i = 0; i < NUM_BUILTINS; i++) {
        is_builtin = strcmp(arguments, builtins[i].name) == 0;
        if (is_builtin) break;
    }

    if (is_builtin) {
        printf("%s is a shell builtin\n", arguments);
    } else {
        printf("%s: not found\n", arguments);
    }

    return false;
}

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (true) {
        printf("$ ");

        char command[SHELL_MAX_CHARS];
        fgets(command, SHELL_MAX_CHARS, stdin);
        command[strlen(command) - 1] = '\0';

        const builtin_t *builtin = NULL;
        for (int i = 0; i < NUM_BUILTINS; i++) {
            bool found = false;
            if (builtins[i].needs_args) {
                char cmp_name[SHELL_MAX_CHARS];
                strcpy(cmp_name, builtins[i].name);
                size_t len_name = strlen(builtins[i].name);
                strcpy(cmp_name + len_name, " ");

                found = !strncmp(cmp_name, command, 5);
            } else {
                found = !strcmp(builtins[i].name, command);
            }

            if (found) {
                builtin = builtins + i;
                break;
            }
        }

        if (builtin) {
            if (builtin->program(command + strlen(builtin->name) + 1))
                break;
            else
                continue;
        }

        // error message
        printf("%s: command not found\n", command);
    }

    return 0;
}
