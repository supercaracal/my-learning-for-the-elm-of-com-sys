#include "vm-parser-pop.h"

static char *conv_pop_static(const struct command *cmd);
static char *conv_pop_fixed_seg(const struct command *cmd, int base);
static char *conv_pop_seg(const struct command *cmd, const char *seg);

char *
conv_pop(const struct command *cmd) {
  if (strcmp(cmd->arg1, "argument") == 0) {
    return conv_pop_seg(cmd, SEG_ARG_REG_NAME);
  } else if (strcmp(cmd->arg1, "local") == 0) {
    return conv_pop_seg(cmd, SEG_LCL_REG_NAME);
  } else if (strcmp(cmd->arg1, "static") == 0) {
    return conv_pop_static(cmd);
  } else if (strcmp(cmd->arg1, "this") == 0) {
    return conv_pop_seg(cmd, SEG_THIS_REG_NAME);
  } else if (strcmp(cmd->arg1, "that") == 0) {
    return conv_pop_seg(cmd, SEG_THAT_REG_NAME);
  } else if (strcmp(cmd->arg1, "pointer") == 0) {
    return conv_pop_fixed_seg(cmd, SEG_PTR_RAM_BASE);
  } else if (strcmp(cmd->arg1, "temp") == 0) {
    return conv_pop_fixed_seg(cmd, SEG_TMP_RAM_BASE);
  } else {
    return "// [ERROR] Unknown segment was detected with pop.";
  }
}

static char *
conv_pop_static(const struct command *cmd) {
  int size;
  char *buf;

  size = 10 * 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M"      "\n"
    "@%s%d.%s" "\n"
    "M=D", SEG_STATIC_LABEL_PREFIX, cmd->fid, cmd->arg2);
  return buf;
}

static char *
conv_pop_fixed_seg(const struct command *cmd, int base) {
  int size;
  char *buf;

  size = 10 * 18;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M"   "\n"
    "@R13"  "\n"
    "M=D"   "\n"

    "@%s"   "\n"
    "D=A"   "\n"
    "@%d"   "\n"
    "A=D+A" "\n"
    "D=A"   "\n"
    "@14"   "\n"
    "M=D"   "\n"

    "@13"   "\n"
    "D=M"   "\n"
    "@14"   "\n"
    "A=M"   "\n"
    "M=D", cmd->arg2, base);
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
