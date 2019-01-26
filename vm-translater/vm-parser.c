#include "vm-parser.h"

static char *conv_cmpr(const char *jump, struct label *lbl);

char *
parse_vm_command(const struct command *cmd, struct label *lbl) {
  switch (cmd->type) {
    case ADD:  return STK_TWICE_POP "M=D+M" STK_FW_SP;
    case SUB:  return STK_TWICE_POP "M=M-D" STK_FW_SP;
    case NEG:  return STK_POP "M=-M" STK_FW_SP;
    case EQ:   return conv_cmpr("JEQ", lbl);
    case GT:   return conv_cmpr("JGT", lbl);
    case LT:   return conv_cmpr("JLT", lbl);
    case AND:  return STK_TWICE_POP "M=D&M" STK_FW_SP;
    case OR:   return STK_TWICE_POP "M=D|M" STK_FW_SP;
    case NOT:  return STK_POP "M=!M" STK_FW_SP;
    case PUSH: return conv_push(cmd);
    case POP:  return conv_pop(cmd);
    default:   return "// [ERROR] unknown command detected";
  }
}

char *
stringify_command(enum cmd_type type) {
  switch (type) {
    case ADD:  return "add";
    case SUB:  return "sub";
    case NEG:  return "neg";
    case EQ:   return "eq";
    case GT:   return "gt";
    case LT:   return "lt";
    case AND:  return "and";
    case OR:   return "or";
    case NOT:  return "not";
    case PUSH: return "push";
    case POP:  return "pop";
    default:   return "unknown";
  }
}

char *
build_finish_command(struct label *lbl) {
  int size;
  char *label;
  char *buf;

  size = 64;
  buf = asm_code_alloc(size);
  label = create_label(lbl);
  snprintf(buf, size,
    "(%s)" "\n"
    "@%s"  "\n"
    "0;JMP", label, label);
  return buf;
}

static char *
conv_cmpr(const char *jump, struct label *lbl) {
  int size;
  char *buf;
  char *label_true;
  char *label_last;

  size = 256;
  buf = asm_code_alloc(size);
  label_true = create_label(lbl);
  label_last = create_label(lbl);
  snprintf(buf, size,
    STK_TWICE_POP
    "D=M-D" "\n"
    "@%s"   "\n"
    "D;%s"  "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=0"   "\n"
    "@%s"   "\n"
    "0;JMP" "\n"
    "(%s)"  "\n"
    "@SP"   "\n"
    "A=M"   "\n"
    "M=-1"  "\n"
    "(%s)"
    STK_FW_SP, label_true, jump, label_last, label_true, label_last);
  return buf;
}
