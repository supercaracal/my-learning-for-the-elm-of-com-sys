#ifndef VM_TRANSLATER_VM_LEXER_H_
#define VM_TRANSLATER_VM_LEXER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_types.h"

void lex_vm_files(struct vm_list *vl, struct cmd_list *cl);
void cl_free(struct cmd_list *cl);

#endif  // VM_TRANSLATER_VM_LEXER_H_
