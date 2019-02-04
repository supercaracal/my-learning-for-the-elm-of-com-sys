#include "vm-parser.h"

static char *conv_cmpr(const char *jump, struct label *lbl);
static char *conv_label(const struct command *cmd);
static char *conv_goto(const struct command *cmd);
static char *conv_if_goto(const struct command *cmd);
static char *conv_function(const struct command *cmd);
static char *conv_return(const struct command *cmd);

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
    case LABEL:    return conv_label(cmd);
    case GOTO:     return conv_goto(cmd);
    case IFGOTO:   return conv_if_goto(cmd);
    case FUNCTION: return conv_function(cmd);
    case CALL:     return "// [ERROR] not implemented yet: call";
    case RETURN:   return conv_return(cmd);
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
conv_label(const struct command *cmd) {
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
conv_goto(const struct command *cmd) {
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
conv_if_goto(const struct command *cmd) {
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

static char *
conv_function(const struct command *cmd) {
  int size;
  char *buf;
  char *lcl_buf;
  int i;
  int lcl_n;
  int lcl_len;
  int lbl_len;

  lbl_len = strlen(cmd->arg1);
  for (i = 0; i < lbl_len; ++i) cmd->arg1[i] = toupper(cmd->arg1[i]);
  lcl_n = atoi(cmd->arg2);
  lcl_len = 70 * lcl_n;
  lcl_buf = asm_code_alloc(lcl_len + 1);
  for (i = 0, size = 70; i < lcl_n; ++i) {
    buf = asm_code_alloc(size);
    snprintf(buf, size,
      "\n" "@%d"
      "\n" "D=A"
      "\n" "@LCL"
      "\n" "A=D+M"
      "\n" "M=0"
      STK_FW_SP, i);
    strcat(lcl_buf, buf);  // NOLINT
  }
  size = 2 + lbl_len + lcl_len + 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size, "(%s)%s", cmd->arg1, lcl_buf);
  return buf;
}

#define BACK_CALLER_SEG \
  "@R13"  "\n"\
  "M=M-1" "\n"\
  "A=M"   "\n"\
  "D=M"   "\n"

static char *
conv_return(const struct command *cmd) {
  return
    "@LCL"  "\n"
    "D=M"   "\n"
    "@R13"  "\n"
    "M=D"   "\n"  // R13 = FRAME = LCL
    "@5"    "\n"
    "D=A"   "\n"
    "@R13"  "\n"
    "A=M-D" "\n"  // FRAME - 5
    "D=M"   "\n"  // *(FRAME - 5)
    "@R14"  "\n"
    "M=D"   "\n"  // R14 = RET_ADDR = *(FRAME - 5)
    STK_POP
    "D=M"   "\n"
    "@ARG"  "\n"
    "A=M"   "\n"
    "M=D"   "\n"  // *ARG = RET_VAL = pop()
    "@ARG"  "\n"
    "D=M"   "\n"
    "@SP"   "\n"
    "M=D+1" "\n"  // SP = ARG + 1
    BACK_CALLER_SEG
    "@THAT" "\n"
    "M=D"   "\n"  // THAT = *(FRAME - 1)
    BACK_CALLER_SEG
    "@THIS" "\n"
    "M=D"   "\n"  // THIS = *(FRAME - 2)
    BACK_CALLER_SEG
    "@ARG"  "\n"
    "M=D"   "\n"  // ARG = *(FRAME - 3)
    BACK_CALLER_SEG
    "@LCL"  "\n"
    "M=D"   "\n"  // LCL = *(FRAME - 4)
    "@R14"  "\n"
    "A=M"   "\n"
    "0;JMP";      // goto RET_ADDR
}
