#ifndef __PARSER_H__
#define __PARSER_H__

#include "shell.h"
#include <stdbool.h>
#include <stddef.h>

bool parse(command_t *command_out, char *input);

#endif // !__PARSER_H__
