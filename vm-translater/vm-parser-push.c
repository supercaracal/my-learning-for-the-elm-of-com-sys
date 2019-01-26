#include "vm-parser-push.h"

static char *conv_push_argument(const struct command *cmd);
static char *conv_push_local(const struct command *cmd);
static char *conv_push_static(const struct command *cmd);
static char *conv_push_constant(const struct command *cmd);
static char *conv_push_this(const struct command *cmd);
static char *conv_push_that(const struct command *cmd);
static char *conv_push_pointer(const struct command *cmd);
static char *conv_push_temp(const struct command *cmd);

char *
conv_push(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_push_argument(cmd);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_push_local(cmd);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_push_static(cmd);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_push_constant(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_push_this(cmd);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_push_that(cmd);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_push_pointer(cmd);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_push_temp(cmd);
  } else {
    return "Unknown segment";
  }
}

static char *
conv_push_argument(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_push_local(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_push_static(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
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
conv_push_this(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_push_that(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_push_pointer(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_push_temp(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}
