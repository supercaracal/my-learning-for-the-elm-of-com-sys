#include "vm-parser-pop.h"

static char *conv_pop_static(const struct command *cmd);
static char *conv_pop_pointer(const struct command *cmd);
static char *conv_pop_temp(const struct command *cmd);
static char *conv_pop_seg(const struct command *cmd, const char *seg);

char *
conv_pop(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_pop_seg(cmd, "ARG");
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_pop_seg(cmd, "LCL");
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_pop_static(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_pop_seg(cmd, "THIS");
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_pop_seg(cmd, "THAT");
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_pop_pointer(cmd);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_pop_temp(cmd);
  } else {
    return "// [ERROR] unknown segment detected at pop";
  }
}

static char *
conv_pop_static(const struct command *cmd) {
  // TODO(T.K): impl
  return "// Not implemented yet.";
}

static char *
conv_pop_pointer(const struct command *cmd) {
  // TODO(T.K): impl
  return "// Not implemented yet.";
}

static char *
conv_pop_temp(const struct command *cmd) {
  int size;
  char *buf;

  size = 10 * 19;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M"   "\n"
    "@R13"  "\n"
    "M=D"   "\n"

    "@%s"   "\n"
    "D=A"   "\n"
    "@5"    "\n"
    "A=D+A" "\n"
    "D=A"   "\n"
    "@14"   "\n"
    "M=D"   "\n"

    "@13"   "\n"
    "D=M"   "\n"
    "@14"   "\n"
    "A=M"   "\n"
    "M=D", cmd->arg2);
  return buf;
}

static char *
conv_pop_seg(const struct command *cmd, const char *seg) {
  int size;
  char *buf;

  size = 10 * 19;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M"   "\n"
    "@R13"  "\n"
    "M=D"   "\n"

    "@%s"   "\n"
    "D=A"   "\n"
    "@%s"   "\n"
    "A=M"   "\n"
    "A=D+A" "\n"
    "D=A"   "\n"
    "@14"   "\n"
    "M=D"   "\n"

    "@13"   "\n"
    "D=M"   "\n"
    "@14"   "\n"
    "A=M"   "\n"
    "M=D", cmd->arg2, seg);
  return buf;
}
