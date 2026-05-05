#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SHELL_MAX_CHARS 256

const char * const builtins[3] = {
    "exit",
    "echo",
    "type"
};

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (true) {
        printf("$ ");

        char command[SHELL_MAX_CHARS];
        fgets(command, SHELL_MAX_CHARS, stdin);
        command[strlen(command) - 1] = '\0';

        if (!strcmp(command, "exit")) {
            break;
        }

        if (!strncmp(command, "echo ", 5)) {
            printf("%s\n", command + 5);
        } else if (!strncmp(command, "type ", 5)) {
            size_t num_builtins = sizeof(builtins) / sizeof(char *);
            char *arg = command + 5;
            bool is_builtin = false;

            for (size_t i = 0; i < num_builtins; i++) {
                 is_builtin = strcmp(arg, builtins[i]) == 0;
                 if (is_builtin) break;
            }

            if (is_builtin) {
                printf("%s is a shell builtin\n", arg);
            } else {
                printf("%s: not found\n", arg);
            }
        } else {
            // error message
            printf("%s: command not found\n", command);
        }
    }

    return 0;
}
