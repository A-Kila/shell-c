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

        // locate builtin commands
        const builtin_t *builtin = NULL;
        if (find_builtin(&builtin, command.argv[0])) {
            if (builtin->program(&command))
                break;
            else
                continue;
        }

        if (execute_external(&command)) 
            continue;

        printf("%s: command not found\n", command.argv[0]);
    }

    return 0;
}
