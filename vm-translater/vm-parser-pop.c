#include "vm-parser-pop.h"

static char *conv_pop_argument(const struct command *cmd);
static char *conv_pop_local(const struct command *cmd);
static char *conv_pop_static(const struct command *cmd);
static char *conv_pop_constant(const struct command *cmd);
static char *conv_pop_this(const struct command *cmd);
static char *conv_pop_that(const struct command *cmd);
static char *conv_pop_pointer(const struct command *cmd);
static char *conv_pop_temp(const struct command *cmd);

char *
conv_pop(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_pop_argument(cmd);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_pop_local(cmd);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_pop_static(cmd);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_pop_constant(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_pop_this(cmd);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_pop_that(cmd);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_pop_pointer(cmd);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_pop_temp(cmd);
  } else {
    return "Unknown segment";
  }
}

static char *
conv_pop_argument(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_local(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_static(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_constant(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_this(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_that(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_pointer(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}

static char *
conv_pop_temp(const struct command *cmd) {
  // TODO(T.K): impl
  return "Not implemented yet.";
}
