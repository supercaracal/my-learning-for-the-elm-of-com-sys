#ifndef VM_TRANSLATER_VM_PARSER_COMMON_H_
#define VM_TRANSLATER_VM_PARSER_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#define STK_POP \
  "@SP"   "\n"\
  "M=M-1" "\n"\
  "A=M"   "\n"

#define STK_TWICE_POP \
  "@SP"   "\n"\
  "M=M-1" "\n"\
  "A=M"   "\n"\
  "D=M"   "\n"\
  "@SP"   "\n"\
  "M=M-1" "\n"\
  "A=M"   "\n"

#define STK_FW_SP "\n"\
  "@SP"   "\n"\
  "M=M+1"

#define SEG_ARG_REG_NAME "ARG"
#define SEG_LCL_REG_NAME "LCL"
#define SEG_THIS_REG_NAME "THIS"
#define SEG_THAT_REG_NAME "THAT"
#define SEG_PTR_RAM_BASE 3
#define SEG_TMP_RAM_BASE 5
#define SEG_STATIC_LABEL_PREFIX "STATIC"

char *asm_code_alloc(int size);

#endif  // VM_TRANSLATER_VM_PARSER_COMMON_H_
