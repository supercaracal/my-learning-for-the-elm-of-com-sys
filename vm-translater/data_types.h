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
  PUSH, POP,
  LABEL, GOTO, IFGOTO,
  FUNCTION, CALL, RETURN
};

struct command {
  enum cmd_type type;
  char *arg1;
  char *arg2;
  int fid;  // vm file id
};

struct cmd_list {
  struct command **cmds;
  int size;
  int idx;
};

struct label {
  char *prefix;
  int i;
};

#endif  // VM_TRANSLATER_DATA_TYPES_H_
