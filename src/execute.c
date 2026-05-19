#include "execute.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_external(const command_t *command) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command->argv[0], command->argv);

        // perror(command->argv[0]);
        printf("%s: command not found\n", command->argv[0]);
 
        exit(1);
    }
 
    waitpid(pid, NULL, 0);
}
