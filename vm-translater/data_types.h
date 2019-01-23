#ifndef VM_TRANSLATER_DATA_TYPES_H_
#define VM_TRANSLATER_DATA_TYPES_H_

struct path_list {
  char **paths;
  int size;
  int idx;
};

struct vm_list {
  char **bufs;
  int size;
  int idx;
};

enum cmd_type {
  UNKNOWN,
  ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT,
  PUSH, POP
};

struct command {
  enum cmd_type type;
  char *arg1;
  char *arg2;
};

struct cmd_list {
  struct command **cmds;
  int size;
  int idx;
};

struct stack {
  int **array;
  int sp;  // the stack pointer
};

#endif  // VM_TRANSLATER_DATA_TYPES_H_
