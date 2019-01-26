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

char *asm_code_alloc(int size);

#endif  // VM_TRANSLATER_VM_PARSER_COMMON_H_
