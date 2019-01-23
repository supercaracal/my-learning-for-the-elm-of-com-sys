#ifndef VM_TRANSLATER_VM_STACK_H_
#define VM_TRANSLATER_VM_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "data_types.h"

struct stack *stk_alloc(void);
void stk_free(struct stack *stk);
void stk_push(struct stack *stk, int value);
int stk_pop(struct stack *stk);

#endif  // VM_TRANSLATER_VM_STACK_H_
