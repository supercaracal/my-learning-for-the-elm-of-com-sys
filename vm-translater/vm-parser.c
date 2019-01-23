#include "vm-parser.h"

#define FIRST_STK_MEM_ADDR 256

static char *asm_code_alloc(int size);
static char *conv_add(struct command *cmd, struct stack *stk);
static char *conv_sub(struct command *cmd, struct stack *stk);
static char *conv_neg(struct command *cmd, struct stack *stk);
static char *conv_eq(struct command *cmd, struct stack *stk);
static char *conv_gt(struct command *cmd, struct stack *stk);
static char *conv_lt(struct command *cmd, struct stack *stk);
static char *conv_and(struct command *cmd, struct stack *stk);
static char *conv_or(struct command *cmd, struct stack *stk);
static char *conv_not(struct command *cmd, struct stack *stk);
static char *conv_push(struct command *cmd, struct stack *stk);
static char *conv_push_argument(struct command *cmd, struct stack *stk);
static char *conv_push_local(struct command *cmd, struct stack *stk);
static char *conv_push_static(struct command *cmd, struct stack *stk);
static char *conv_push_constant(struct command *cmd, struct stack *stk);
static char *conv_push_this(struct command *cmd, struct stack *stk);
static char *conv_push_that(struct command *cmd, struct stack *stk);
static char *conv_push_pointer(struct command *cmd, struct stack *stk);
static char *conv_push_temp(struct command *cmd, struct stack *stk);
static char *conv_pop(struct command *cmd, struct stack *stk);
static char *conv_pop_argument(struct command *cmd, struct stack *stk);
static char *conv_pop_local(struct command *cmd, struct stack *stk);
static char *conv_pop_static(struct command *cmd, struct stack *stk);
static char *conv_pop_constant(struct command *cmd, struct stack *stk);
static char *conv_pop_this(struct command *cmd, struct stack *stk);
static char *conv_pop_that(struct command *cmd, struct stack *stk);
static char *conv_pop_pointer(struct command *cmd, struct stack *stk);
static char *conv_pop_temp(struct command *cmd, struct stack *stk);

char *
parse_vm_command(struct command *cmd, struct stack *stk)
{
  switch (cmd->type) {
    case ADD:
      return conv_add(cmd, stk);
    case SUB:
      return conv_sub(cmd, stk);
    case NEG:
      return conv_neg(cmd, stk);
    case EQ:
      return conv_eq(cmd, stk);
    case GT:
      return conv_gt(cmd, stk);
    case LT:
      return conv_lt(cmd, stk);
    case AND:
      return conv_and(cmd, stk);
    case OR:
      return conv_or(cmd, stk);
    case NOT:
      return conv_not(cmd, stk);
    case PUSH:
      return conv_push(cmd, stk);
    case POP:
      return conv_pop(cmd, stk);
    default:
      return "Unknown command";
  }
}

char *
stringify_command(enum cmd_type type)
{
  switch (type) {
    case ADD:
      return "add";
    case SUB:
      return "sub";
    case NEG:
      return "neg";
    case EQ:
      return "eq";
    case GT:
      return "gt";
    case LT:
      return "lt";
    case AND:
      return "and";
    case OR:
      return "or";
    case NOT:
      return "not";
    case PUSH:
      return "push";
    case POP:
      return "pop";
    default:
      return "unknown";
  }
}

static char *
asm_code_alloc(int size)
{
  char *buf;

  buf = (char *) malloc(size);
  if (buf == NULL) fprintf(stderr, "Could not allocate memory for asm codes.\n");
  return buf;
}

static char *
conv_add(struct command *cmd, struct stack *stk)
{
  char *buf;
  int value;

  value = stk_pop(stk);
  value += stk_pop(stk);
  stk_push(stk, value);
  buf = asm_code_alloc(10 * 13);
  sprintf(
    buf,
    "@SP"   "\n"
    "M=M-1" "\n"
    "A=M"   "\n"
    "D=M"   "\n"
    "@SP"   "\n"
    "M=M-1" "\n"
    "A=M"   "\n"
    "D=D+M" "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=D"   "\n"
    "@SP"   "\n"
    "M=M+1"
  );
  return buf;
}

static char *
conv_sub(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_neg(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_eq(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_gt(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_lt(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_and(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_or(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_not(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push(struct command *cmd, struct stack *stk)
{
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_push_argument(cmd, stk);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_push_local(cmd, stk);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_push_static(cmd, stk);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_push_constant(cmd, stk);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_push_this(cmd, stk);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_push_that(cmd, stk);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_push_pointer(cmd, stk);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_push_temp(cmd, stk);
  } else {
    return "Unknown segment";
  }
}

static char *
conv_push_argument(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_local(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_static(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_constant(struct command *cmd, struct stack *stk)
{
  char *buf;

  buf = asm_code_alloc(10 * 7);
  sprintf(
    buf,
    "@%s" "\n"
    "D=A" "\n"
    "@SP" "\n"
    "A=M" "\n"
    "M=D" "\n"
    "@SP" "\n"
    "M=M+1",
    cmd->arg2
  );
  stk_push(stk, atoi(cmd->arg2));
  return buf;
}

static char *
conv_push_this(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_that(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_pointer(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_push_temp(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop(struct command *cmd, struct stack *stk)
{
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_pop_argument(cmd, stk);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_pop_local(cmd, stk);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_pop_static(cmd, stk);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_pop_constant(cmd, stk);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_pop_this(cmd, stk);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_pop_that(cmd, stk);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_pop_pointer(cmd, stk);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_pop_temp(cmd, stk);
  } else {
    return "Unknown segment";
  }
}

static char *
conv_pop_argument(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_local(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_static(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_constant(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_this(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_that(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_pointer(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}

static char *
conv_pop_temp(struct command *cmd, struct stack *stk)
{
  // TODO: impl
  return "Not implemented yet.";
}
