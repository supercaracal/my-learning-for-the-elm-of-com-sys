#include "vm-parser-push.h"

static char *conv_push_static(const struct command *cmd);
static char *conv_push_constant(const struct command *cmd);
static char *conv_push_fixed_seg(const struct command *cmd, int base);
static char *conv_push_seg(const struct command *cmd, const char *seg);

char *
conv_push(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_push_seg(cmd, SEG_ARG_REG_NAME);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_push_seg(cmd, SEG_LCL_REG_NAME);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_push_static(cmd);
  } else if (strcmp(cmd->arg1, "constant") == 0) {
    return conv_push_constant(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_push_seg(cmd, SEG_THIS_REG_NAME);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_push_seg(cmd, SEG_THAT_REG_NAME);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_push_fixed_seg(cmd, SEG_PTR_RAM_BASE);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_push_fixed_seg(cmd, SEG_TMP_RAM_BASE);
  } else {
    return "// [ERROR] Unknown segment was detected with push.";
  }
}

static char *
conv_push_static(const struct command *cmd) {
  int size;
  char *buf;

  size = 10 * 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s%d.%s" "\n"
    "D=M"      "\n"
    "@SP"      "\n"
    "A=M"      "\n"
    "M=D"
    STK_FW_SP, SEG_STATIC_LABEL_PREFIX, cmd->fid, cmd->arg2);
  return buf;
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
conv_push_fixed_seg(const struct command *cmd, int base) {
  int size;
  char *buf;

  size = 10 * 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s"   "\n"
    "D=A"   "\n"
    "@%d"   "\n"
    "A=D+A" "\n"
    "D=M"   "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=D"
    STK_FW_SP, cmd->arg2, base);
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
