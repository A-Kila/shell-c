#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_MAX_CHARS 256

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (1) {
        printf("$ ");

        char command[SHELL_MAX_CHARS];
        fgets(command, SHELL_MAX_CHARS, stdin);
        command[strlen(command) - 1] = '\0';

        if (!strcmp(command, "exit")) {
            break;
        }

        if (!strncmp(command, "echo ", 5)) {
            printf("%s\n", command + 5);
        } else {
            // error message
            printf("%s: command not found\n", command);
        }
    }

    return 0;
}
