#include "execute.h"
#include "shell.h"

#include "builtins.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (true) {
        printf("$ ");

        char input[BUFFER_SIZE];
        if (!fgets(input, BUFFER_SIZE, stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        command_t command;
        if (!parse(&command, input)) continue;

        // locate builtin commands; run if found;
        const builtin_t *builtin = find_builtin(command.argv[0]);
        if (builtin) {
            builtin->program(&command);
            continue;
        }

        execute_external(&command);

        command_free(&command);
    }

    return 0;
}
