#include "execute.h"

#include "utils.h"
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>

bool execute_external(const command_t *command) {
    if (!find_program(NULL, command->argv[0]))
        return false;

    pid_t pid = fork();
    if (pid == 0) {
        execvp(command->argv[0], command->argv);
    } else {
        waitpid(pid, NULL, 0);
    }

    return true;
}
