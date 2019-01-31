#include "vm-parser.h"

static char *conv_cmpr(const char *jump, struct label *lbl);
static char *conv_label(const struct command *cmd, struct label *lbl);
static char *conv_goto(const struct command *cmd, struct label *lbl);
static char *conv_if_goto(const struct command *cmd, struct label *lbl);

char *
parse_vm_command(const struct command *cmd, struct label *lbl) {
  switch (cmd->type) {
    case ADD:      return STK_TWICE_POP "M=D+M" STK_FW_SP;
    case SUB:      return STK_TWICE_POP "M=M-D" STK_FW_SP;
    case NEG:      return STK_POP "M=-M" STK_FW_SP;
    case EQ:       return conv_cmpr("JEQ", lbl);
    case GT:       return conv_cmpr("JGT", lbl);
    case LT:       return conv_cmpr("JLT", lbl);
    case AND:      return STK_TWICE_POP "M=D&M" STK_FW_SP;
    case OR:       return STK_TWICE_POP "M=D|M" STK_FW_SP;
    case NOT:      return STK_POP "M=!M" STK_FW_SP;
    case PUSH:     return conv_push(cmd);
    case POP:      return conv_pop(cmd);
    case LABEL:    return conv_label(cmd, lbl);
    case GOTO:     return conv_goto(cmd, lbl);
    case IFGOTO:   return conv_if_goto(cmd, lbl);
    case FUNCTION: return "// [ERROR] not implemented yet: function";
    case CALL:     return "// [ERROR] not implemented yet: call";
    case RETURN:   return "// [ERROR] not implemented yet: return";
    default:       return "// [ERROR] unknown command detected";
  }
}

char *
stringify_command(enum cmd_type type) {
  switch (type) {
    case ADD:      return "add";
    case SUB:      return "sub";
    case NEG:      return "neg";
    case EQ:       return "eq";
    case GT:       return "gt";
    case LT:       return "lt";
    case AND:      return "and";
    case OR:       return "or";
    case NOT:      return "not";
    case PUSH:     return "push";
    case POP:      return "pop";
    case LABEL:    return "label";
    case GOTO:     return "goto";
    case IFGOTO:   return "if-goto";
    case FUNCTION: return "function";
    case CALL:     return "call";
    case RETURN:   return "return";
    default:       return "unknown";
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

static char *
conv_label(const struct command *cmd, struct label *lbl) {
  int size;
  char *buf;
  int i;
  int lbl_len;

  lbl_len = strlen(cmd->arg1);
  for (i = 0; i < lbl_len; ++i) cmd->arg1[i] = toupper(cmd->arg1[i]);
  size = 2 + lbl_len + 1;
  buf = asm_code_alloc(size);
  snprintf(buf, size, "(%s)", cmd->arg1);
  return buf;
}

static char *
conv_goto(const struct command *cmd, struct label *lbl) {
  int size;
  char *buf;
  int i;
  int lbl_len;

  lbl_len = strlen(cmd->arg1);
  for (i = 0; i < lbl_len; ++i) cmd->arg1[i] = toupper(cmd->arg1[i]);
  size = 10 * 2 + lbl_len + 1;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s" "\n"
    "0;JMP", cmd->arg1);
  return buf;
}

static char *
conv_if_goto(const struct command *cmd, struct label *lbl) {
  int size;
  char *buf;
  int i;
  int lbl_len;

  lbl_len = strlen(cmd->arg1);
  for (i = 0; i < lbl_len; ++i) cmd->arg1[i] = toupper(cmd->arg1[i]);
  size = 10 * 6 + lbl_len + 1;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M" "\n"
    "@%s" "\n"
    "D;JGT", cmd->arg1);
  return buf;
}
