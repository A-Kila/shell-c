#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include "shell.h"
#include <stdbool.h>

typedef bool (*builtin_fun_t)(const command_t *);
typedef struct {
        const char *name;
        const builtin_fun_t program;
} builtin_t;

bool find_builtin(const builtin_t **builtin_out, const char *cmd_name);

#endif // !__BUILTINS_H__
