#include "vm-parser-push.h"

static char *conv_push_static(const struct command *cmd);
static char *conv_push_constant(const struct command *cmd);
static char *conv_push_pointer(const struct command *cmd);
static char *conv_push_temp(const struct command *cmd);
static char *conv_push_seg(const struct command *cmd, const char *seg);

char *
conv_push(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_push_seg(cmd, "ARG");
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_push_seg(cmd, "LCL");
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_push_static(cmd);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_push_constant(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_push_seg(cmd, "THIS");
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_push_seg(cmd, "THAT");
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_push_pointer(cmd);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_push_temp(cmd);
  } else {
    return "// [ERROR] unknown segment detected at push";
  }
}

static char *
conv_push_static(const struct command *cmd) {
  // TODO(T.K): impl
  return "// Not implemented yet.";
}

static char *
conv_push_constant(const struct command *cmd) {
  int size;
  char *buf;

  size = 10 * 7;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s" "\n"
    "D=A" "\n"
    "@SP" "\n"
    "A=M" "\n"
    "M=D"
    STK_FW_SP, cmd->arg2);
  return buf;
}

static char *
conv_push_pointer(const struct command *cmd) {
  // TODO(T.K): impl
  return "// Not implemented yet.";
}

static char *
conv_push_temp(const struct command *cmd) {
  int size;
  char *buf;

  size = 10 * 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s"   "\n"
    "D=A"   "\n"
    "@5"    "\n"
    "A=D+A" "\n"
    "D=M"   "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=D"
    STK_FW_SP, cmd->arg2);
  return buf;
}

static char *
conv_push_seg(const struct command *cmd, const char *seg) {
  int size;
  char *buf;

  size = 10 * 11;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s"   "\n"
    "D=A"   "\n"
    "@%s"   "\n"
    "A=M"   "\n"
    "A=D+A" "\n"
    "D=M"   "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=D"
    STK_FW_SP, cmd->arg2, seg);
  return buf;
}
