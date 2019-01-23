#include "vm-stack.h"

#define STACK_SIZE 1792

struct stack *
stk_alloc(void) {
  struct stack *stk;
  int i;

  stk = (struct stack *) malloc(sizeof(struct stack));
  if (stk == NULL) {
    fprintf(stderr, "Could not allocate memory for the vm stack.\n");
    return NULL;
  }

  stk->sp = 0;
  stk->array = (int **) calloc(STACK_SIZE, sizeof(int *));
  if (stk->array == NULL) {
    fprintf(stderr, "Could not allocate memory for the vm stack array.\n");
    free(stk);
    return NULL;
  }

  for (i = 0; i < STACK_SIZE; ++i) {
    stk->array[i] = (int *) malloc(sizeof(int));
    if (stk->array[i] == NULL) {
      fprintf(stderr, "Could not allocate memory for a element in the vm stack.\n");
      break;
    }
  }

  return stk;
}

void
stk_free(struct stack *stk) {
  int i;

  for (i = 0; i < stk->sp; ++i) free(stk->array[i]);
  free(stk);
}

void
stk_push(struct stack *stk, int value) {
  if (stk->sp >= STACK_SIZE) {
    fprintf(stderr, "The vm stack is full. %d was given.\n", value);
  } else {
    *stk->array[stk->sp] = value;
    stk->sp++;
  }
}

int
stk_pop(struct stack *stk) {
  int value;

  if (stk->sp == 0) {
    fprintf(stderr, "The vm stack is empty.\n");
    value = 0;
  } else {
    value = *stk->array[stk->sp - 1];
    stk->sp--;
  }

  return value;
}
