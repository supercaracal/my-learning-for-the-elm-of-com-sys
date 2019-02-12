#include "vm-parser.h"

static char *conv_cmpr(const char *jump, struct label *lbl);
static char *conv_label(const char *lname, char *cfunc);
static char *conv_goto(const char *lname, char *cfunc);
static char *conv_if_goto(const char *lname, char *cfunc);
static char *conv_function(const struct command *cmd, char *cfunc);
static char *conv_return(void);
static char *build_call_asm_code(const char *fname, int narg, struct label *lbl);

char *
parse_vm_command(const struct command *cmd, struct label *lbl, char *cfunc) {
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
    case LABEL:    return conv_label(cmd->arg1, cfunc);
    case GOTO:     return conv_goto(cmd->arg1, cfunc);
    case IFGOTO:   return conv_if_goto(cmd->arg1, cfunc);
    case FUNCTION: return conv_function(cmd, cfunc);
    case CALL:     return build_call_asm_code(cmd->arg1, atoi(cmd->arg2), lbl);
    case RETURN:   return conv_return();
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
build_bootstrap_asm_code(struct label *lbl) {
  int size;
  char *buf;
  char *init_call;

  init_call = build_call_asm_code(INIT_FUNC_NAME, 0, lbl);
  size = 10 * 5 + strlen(init_call) + 1;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%d"   "\n"
    "D=A"   "\n"
    "@SP"   "\n"
    "M=D"   "\n"
    "%s", MIN_SP_ADDR, init_call);
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
conv_label(const char *lname, char *cfunc) {
  int size;
  char *buf;
  int in_fnc;

  size = 2 + strlen(lname) + 1;
  in_fnc = cfunc[0] != '\0';
  if (in_fnc) size += MAX_CUR_FNC_NAME_SIZE;
  buf = asm_code_alloc(size);
  snprintf(buf, size, "(%s%s%s)", (in_fnc ? cfunc : ""), (in_fnc ? "$" : ""), lname);
  return buf;
}

static char *
conv_goto(const char *lname, char *cfunc) {
  int size;
  char *buf;
  int in_fnc;

  size = 10 * 2 + strlen(lname) + 1;
  in_fnc = cfunc[0] != '\0';
  if (in_fnc) size += MAX_CUR_FNC_NAME_SIZE;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    "@%s%s%s" "\n"
    "0;JMP", (in_fnc ? cfunc : ""), (in_fnc ? "$" : ""), lname);
  return buf;
}

static char *
conv_if_goto(const char *lname, char *cfunc) {
  int size;
  char *buf;
  int in_fnc;

  size = 10 * 6 + strlen(lname) + 1;
  in_fnc = cfunc[0] != '\0';
  if (in_fnc) size += MAX_CUR_FNC_NAME_SIZE;
  buf = asm_code_alloc(size);
  snprintf(buf, size,
    STK_POP
    "D=M" "\n"
    "@%s%s%s" "\n"
    "D;JNE", (in_fnc ? cfunc : ""), (in_fnc ? "$" : ""), lname);
  return buf;
}

static char *
conv_function(const struct command *cmd, char *cfunc) {
  int size;
  char *buf;
  char *lcl_buf;
  int i;
  int lcl_n;
  int lcl_len;
  int fnc_len;

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
  fnc_len = strlen(cmd->arg1);
  size = 2 + fnc_len + lcl_len + 10;
  buf = asm_code_alloc(size);
  snprintf(buf, size, "(%s)%s", cmd->arg1, lcl_buf);
  snprintf(cfunc, fnc_len, "%s", cmd->arg1);
  return buf;
}

static char *
conv_return(void) {
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

static char *
build_call_asm_code(const char *fname, int narg, struct label *lbl) {
  int size;
  char *buf;
  char *label;

  size = 10 * 70 + strlen(fname) + MAX_CUR_FNC_NAME_SIZE * 2 + 1;
  buf = asm_code_alloc(size);
  label = create_label(lbl);

  snprintf(buf, size,
    "@%s"     "\n"
    "D=A"     "\n"
    STK_PUSH        // push return-address
    "@LCL"    "\n"
    "D=M"     "\n"
    STK_PUSH        // push LCL
    "@ARG"    "\n"
    "D=M"     "\n"
    STK_PUSH        // push ARG
    "@THIS"   "\n"
    "D=M"     "\n"
    STK_PUSH        // push THIS
    "@THAT"   "\n"
    "D=M"     "\n"
    STK_PUSH        // push THAT
    "@%d"     "\n"
    "D=A"     "\n"
    "@SP"     "\n"
    "D=M-D"   "\n"
    "@ARG"    "\n"
    "M=D"     "\n"  // ARG = SP - n - 5
    "@SP"     "\n"
    "D=M"     "\n"
    "@LCL"    "\n"
    "M=D"     "\n"  // LCL = SP
    "@%s"     "\n"
    "0;JMP"   "\n"  // goto func
    "(%s)"          // (return-address)
    "", label, narg + ARG_PTR_OFFSET_ON_CALL, fname, label);
  return buf;
}
