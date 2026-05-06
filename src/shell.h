#ifndef __SHELL_H__
#define __SHELL_H__

#include <stddef.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 64

typedef struct {
    size_t argc;
    char *argv[MAX_ARGS];
} command_t;

#endif // !__SHELL_H__
