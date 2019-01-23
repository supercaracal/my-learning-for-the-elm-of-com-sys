#ifndef VM_TRANSLATER_VM_PARSER_H_
#define VM_TRANSLATER_VM_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_types.h"
#include "vm-stack.h"

char *parse_vm_command(struct command *cmd, struct stack *stk);
char *stringify_command(enum cmd_type type);

#endif  // VM_TRANSLATER_VM_PARSER_H_
