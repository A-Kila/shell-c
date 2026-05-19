#include "builtins.h"

#include "shell.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_exit(const command_t *);
void echo(const command_t *);
void type(const command_t *);
void pwd(const command_t *);
void cd(const command_t *);

const builtin_t builtins[] = {
    { "exit", my_exit },
    { "echo", echo    },
    { "type", type    },
    { "pwd",  pwd     },
    { "cd",   cd      },
};

static size_t len_builtins() {
    return sizeof(builtins) / sizeof(builtin_t);
}

void my_exit(const command_t *command) {
    int status = (command->argc < 2) ? 0 : atoi(command->argv[1]);

    exit(status);
}

void echo(const command_t *command) {
    for (int i = 1; i < command->argc; i++) {
        if (i > 1) printf(" ");
        printf("%s", command->argv[i]);
    }
    printf("\n");
}

void type(const command_t *command) {
    bool is_builtin = false;
    size_t num_builtins = len_builtins();

    if (command->argc < 2) {
        return;
    }

    for (size_t i = 0; i < num_builtins; i++) {
        is_builtin = strcmp(command->argv[1], builtins[i].name) == 0;
        if (is_builtin)
        break;
    }

    if (is_builtin) {
        printf("%s is a shell builtin\n", command->argv[1]);
        return;
    }

    char path[BUFFER_SIZE];
    if (find_program_path(path, command->argv[1])) {
        printf("%s is %s\n", command->argv[1], path);
        return;
    }

    printf("%s: not found\n", command->argv[1]);
}

void pwd(const command_t *command) {
    char *cwd = getcwd(NULL, 0); // malloc's the right size
    printf("%s\n", cwd);
    free(cwd);
}

void cd(const command_t *command) {
    if (chdir(command->argv[1]) != 0)
        printf("cd: %s: No such file or directory\n", command->argv[1]);
}

const builtin_t *find_builtin(const char *cmd_name) {
    size_t num_builtins = len_builtins();

    for (int i = 0; i < num_builtins; i++) {
        if (!strcmp(builtins[i].name, cmd_name))
            return builtins + i;
    }

    return NULL;
}

