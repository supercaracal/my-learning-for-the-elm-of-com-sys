#include "vm-parser-common.h"

char *
asm_code_alloc(int size) {
  char *buf;

  buf = (char *) malloc(size);
  if (buf == NULL) fprintf(stderr, "Could not allocate memory for asm codes.\n");
  return buf;
}
