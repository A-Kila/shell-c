#include "utils.h"

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool find_program(char *path_buffer, const char *const program) {
    char *path_env = getenv("PATH");
    if (path_env == NULL || strlen(path_env) == 0) return false;

    char *path_var = strdup(path_env);

    for (char *folder = strtok(path_var, ":"); folder != NULL; 
            folder = strtok(NULL, ":")) 
    {
        char file[BUFFER_SIZE];
        snprintf(file, BUFFER_SIZE, "%s/%s", folder, program);

        if (!access(file, X_OK)) {
            if (path_buffer)
                strcpy(path_buffer, file);

            free(path_var);
            return true;
        }
    }

    free(path_var);
    return false;
}
