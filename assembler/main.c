#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WORD_SIZE 16
#define ST_MIN_SIZE 256
#define ST_MIN_VAR_ADDR 16
#define NULL_ADDR -1

enum mode {
  CODE,
  SINGLE_LINE_COMMENT,
  MULTIPLE_LINE_COMMENT
};

struct symbol {
  char *name;
  int addr;
};

struct sym_tbl {
  int size;
  int idx;
  int vidx;
  struct symbol **tbl;
};

static int
calc_file_size(FILE *fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}

static char
filter_code(char p, char c, char n, enum mode *m)
{
  char r;

  r = '\0';
  switch (c) {
    case ' ':
    case '\t':
      break;
    case '\r':
    case '\n':
      if (p != '\0' && p != '\r' && p != '\n') r = '\n';
      break;
    case '/':
      if (n == '/') {
        *m = SINGLE_LINE_COMMENT;
      } else if (n == '*') {
        *m = MULTIPLE_LINE_COMMENT;
      }
      break;
    default:
      r = c;
      break;
  }

  return r;
}

static int
strip_code(char *buf, int size)
{
  enum mode m;
  int i;
  int j;
  char current;
  char next;
  char prev;
  char c;

  for (i = 0, j = 0, prev = '\0', m = CODE; i < size; prev = current, ++i) {
    current = buf[i];
    buf[i] = '\0';
    if (i < size - 1) {
      next = buf[i + 1];
    } else {
      next = '\0';
    }
    switch (m) {
      case CODE:
        c = filter_code(prev, current, next, &m);
        if (c != '\0') buf[j++] = c;
        break;
      case SINGLE_LINE_COMMENT:
        if (current == '\n') {
          m = CODE;
          if (j != 0 && buf[j] != '\n') buf[j++] = '\n';
        }
        break;
      case MULTIPLE_LINE_COMMENT:
        if (prev == '*' && current == '/') {
          m = CODE;
          if (j != 0 && buf[j] != '\n') buf[j++] = '\n';
        }
        break;
      default:
        break;
    }
  }

  return j;
}

static int
count_orders(char *buf, int size)
{
  int i;
  int n;

  for (i = 0, n = 0; i < size; ++i) {
    if (buf[i] == '\n') ++n;
  }

  return n;
}

static void
st_alloc(struct sym_tbl *st)
{
  int i;
  struct symbol **p;

  if (st->tbl == NULL) {
    st->size = ST_MIN_SIZE;
    st->idx = 0;
    st->vidx = ST_MIN_VAR_ADDR;
    st->tbl = (struct symbol **) calloc(ST_MIN_SIZE, sizeof(struct symbol *));
    if (st->tbl == NULL) fprintf(stderr, "Could not allocate memory for symbol table\n");
    for (i = 0; i < st->size; ++i) {
      st->tbl[i] = (struct symbol *) malloc(sizeof(struct symbol));
      if (st->tbl[i] == NULL) fprintf(stderr, "Could not allocate memory for symbol table[%d]\n", i);
    }
  } else if (st->idx >= st->size) {
    st->size *= 2;
    p = (struct symbol **) realloc(st->tbl, st->size * sizeof(struct symbol *));
    if (p == NULL) {
      fprintf(stderr, "Could not reallocate memory for symbol table\n");
    } else {
      st->tbl = p;
      for (i = st->idx; i < st->size; ++i) {
        st->tbl[i] = (struct symbol *) malloc(sizeof(struct symbol));
        if (st->tbl[i] == NULL) fprintf(stderr, "Could not reallocate memory for symbol table[%d]\n", i);
      }
    }
  }
}

static void
st_free(struct sym_tbl *st)
{
  int i;

  for (i = 0; i < st->size; ++i) {
    // FIXME: double free or corruption
    // at MaxL.asm and RectL.asm files
    // if (i < st->idx) free(st->tbl[i]->name);
    free(st->tbl[i]);
  }
  free(st->tbl);
}

static int
st_get(struct sym_tbl *st, char *name)
{
  int i;
  int addr;

  // OPTIMIZE: O(N)
  for (i = 0, addr = NULL_ADDR; i < st->idx; ++i) {
    if (strcmp(st->tbl[i]->name, name) == 0) {
      addr = st->tbl[i]->addr;
      break;
    }
  }

  return addr;
}

static void
st_add(struct sym_tbl *st, char *name, int addr)
{
  int n;

  st_alloc(st);
  n = st_get(st, name);
  if (n == NULL_ADDR) {
    st->tbl[st->idx]->name = name;
    st->tbl[st->idx]->addr = addr;
    st->idx++;
  }
}

static void
st_vadd(struct sym_tbl *st, char *name)
{
  int n;

  st_alloc(st);
  n = st_get(st, name);
  if (n == NULL_ADDR) {
    st->tbl[st->idx]->name = name;
    st->tbl[st->idx]->addr = st->vidx++;
    st->idx++;
  }
}

static void
st_print(struct sym_tbl *st)
{
  int i;

#ifdef DEBUG
  fprintf(stdout, "\nSymbol Table:\n");
#endif
  for (i = 0; i < st->idx; ++i) {
#ifdef DEBUG
    fprintf(stdout, "  %s=%d\n", st->tbl[i]->name, st->tbl[i]->addr);
#endif
  }
}

static void
parse_a_order(char *mnemonic, int size, char word[WORD_SIZE], struct sym_tbl *st)
{
  int i;
  int num;
  char *name;

  ++mnemonic; // skip `@`

  if (*mnemonic >= '0' && *mnemonic <= '9') {
    num = (int) strtol(mnemonic, NULL, 10);
  } else {
    name = (char *) malloc(size);
    if (name == NULL) fprintf(stderr, "Could not allocate memory for symbol name in parsing a-order\n");
    strncpy(name, mnemonic, size - 1);
    num = st_get(st, name);
    if (num == NULL_ADDR) st_vadd(st, name);
    num = st_get(st, name);
  }

  for (i = WORD_SIZE - 1; i >= 0 && num > 0; --i, num /= 2) {
    word[i] = '0' + (num % 2);
  }
}

static void
parse_label(char *mnemonic, int size, int dest_addr, struct sym_tbl *st)
{
  char *name;

  name = (char *) malloc(size - 1);
  if (name == NULL) fprintf(stderr, "Could not allocate memory for symbol name in parsing label\n");
  strncpy(name, ++mnemonic, size - 2);
  st_add(st, name, dest_addr);
}

static void
parse_computation(char comp[3], char word[WORD_SIZE])
{
  if (strcmp(comp, "0") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '0'; word[6] = '1'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "1") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '1'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "-1") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '1'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "D") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '1'; word[7] = '1'; word[8] = '0'; word[9] = '0';
  } else if (strcmp(comp, "A") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '0';
  } else if (strcmp(comp, "!D") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '1'; word[7] = '1'; word[8] = '0'; word[9] = '1';
  } else if (strcmp(comp, "!A") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '1';
  } else if (strcmp(comp, "-D") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '1'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "-A") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "D+1") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '1'; word[6] = '1'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "A+1") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "D-1") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '1'; word[7] = '1'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "A-1") == 0) {
    word[3] = '0'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "D+A") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "D-A") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "A-D") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "D&A") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '0';
  } else if (strcmp(comp, "D|A") == 0) {
    word[3] = '0'; word[4] = '0'; word[5] = '1'; word[6] = '0'; word[7] = '1'; word[8] = '0'; word[9] = '1';
  } else if (strcmp(comp, "M") == 0) {
    word[3] = '1'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '0';
  } else if (strcmp(comp, "!M") == 0) {
    word[3] = '1'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '1';
  } else if (strcmp(comp, "-M") == 0) {
    word[3] = '1'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "M+1") == 0) {
    word[3] = '1'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "M-1") == 0) {
    word[3] = '1'; word[4] = '1'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "D+M") == 0) {
    word[3] = '1'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '0';
  } else if (strcmp(comp, "D-M") == 0) {
    word[3] = '1'; word[4] = '0'; word[5] = '1'; word[6] = '0'; word[7] = '0'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "M-D") == 0) {
    word[3] = '1'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '1'; word[8] = '1'; word[9] = '1';
  } else if (strcmp(comp, "D&M") == 0) {
    word[3] = '1'; word[4] = '0'; word[5] = '0'; word[6] = '0'; word[7] = '0'; word[8] = '0'; word[9] = '0';
  } else if (strcmp(comp, "D|M") == 0) {
    word[3] = '1'; word[4] = '0'; word[5] = '1'; word[6] = '0'; word[7] = '1'; word[8] = '0'; word[9] = '1';
  }
}

static void
parse_destination(char dest[3], char word[WORD_SIZE])
{
  if (dest[0] == 'A' || dest[1] == 'A' || dest[2] == 'A') word[10] = '1';
  if (dest[0] == 'D' || dest[1] == 'D' || dest[2] == 'D') word[11] = '1';
  if (dest[0] == 'M' || dest[1] == 'M' || dest[2] == 'M') word[12] = '1';
}

static void
parse_jump(char jump[3], char word[WORD_SIZE])
{
  if (strcmp(jump, "JGT") == 0) {
    word[13] = '0'; word[14] = '0'; word[15] = '1';
  } else if (strcmp(jump, "JEQ") == 0) {
    word[13] = '0'; word[14] = '1'; word[15] = '0';
  } else if (strcmp(jump, "JGE") == 0) {
    word[13] = '0'; word[14] = '1'; word[15] = '1';
  } else if (strcmp(jump, "JLT") == 0) {
    word[13] = '1'; word[14] = '0'; word[15] = '0';
  } else if (strcmp(jump, "JNE") == 0) {
    word[13] = '1'; word[14] = '0'; word[15] = '1';
  } else if (strcmp(jump, "JLE") == 0) {
    word[13] = '1'; word[14] = '1'; word[15] = '0';
  } else if (strcmp(jump, "JMP") == 0) {
    word[13] = '1'; word[14] = '1'; word[15] = '1';
  }
}

static void
parse_c_order(char *mnemonic, int size, char word[WORD_SIZE])
{
  char dest[3] = { '\0', '\0', '\0' };
  char comp[3] = { '\0', '\0', '\0' };
  char jump[3] = { '\0', '\0', '\0' };
  char *p;
  int equal;
  int semicolon;
  int i;

  word[0] = '1';
  word[1] = '1';
  word[2] = '1';

  for (p = mnemonic, equal = 0, semicolon = 0, i = 0; p < mnemonic + size; ++p) {
    if (*p == '=') {
      equal = 1; comp[0] = '\0'; comp[1] = '\0'; comp[2] = '\0'; i = 0;
    } else if (*p == ';') {
      semicolon = 1; dest[0] = '\0'; dest[1] = '\0'; dest[2] = '\0'; i = 0;
    } else {
      if (semicolon) {
        jump[i++] = *p;
      } else if (equal) {
        comp[i++] = *p;
      } else {
        dest[i] = *p; comp[i] = *p; ++i;
      }
    }
  }

  parse_computation(comp, word);
  parse_destination(dest, word);
  parse_jump(jump, word);
}

static int
parse_code(char *buf, int size, char *bin, struct sym_tbl *st)
{
  int i;
  int j;
  int k;
  int first;
  char word[WORD_SIZE];

  for (i = 0, j = 0, first = 0; i < size; ++i) {
    if (buf[i] != '\n') continue;
    if (buf[first] == '(') {
      parse_label(&buf[first], i - first, j, st);
    } else {
      ++j; // count order
    }
    first = i + 1;
  }

  for (i = 0, j = 0, first = 0; i < size; ++i) {
    if (buf[i] != '\n') continue;
    if (buf[first] == '(') {
        first = i + 1;
        continue;
    }
    for (k = 0; k < WORD_SIZE; ++k) word[k] = '0';
    switch (buf[first]) {
      case '@':
        parse_a_order(&buf[first], i - first, word, st);
        break;
      default:
        parse_c_order(&buf[first], i - first, word);
        break;
    }
    for (k = 0; k < WORD_SIZE; ++k) bin[j++] = word[k];
    bin[j++] = '\n';
    first = i + 1;
  }

  return j;
}

static void
add_reserved_symbols(struct sym_tbl *st)
{
  int i;
  int j;
  int length;
  char *name;
  char names[] = "SP,LCL,ARG,THIS,THAT,SCREEN,KBD,R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,";
  int addrs[23] = { 0, 1, 2, 3, 4, 16384, 24576, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

  for (i = 0, j = 0, length = 0; names[i] != '\0'; ++i) {
    if (names[i] != ',') {
      ++length;
      continue;
    }
    name = (char *) malloc(length + 1);
    if (name == NULL) fprintf(stderr, "Could not allocate memory for a reserved symbol name\n");
    strncpy(name, &names[i - length], length);
    name[length] = '\0';
    st_add(st, name, addrs[j++]);
    length = 0;
  }
}

int
main(int argc, char *argv[])
{
  FILE *fp;
  char *buf;
  char *bin;
  int size;
  int n;
  struct sym_tbl st = { ST_MIN_SIZE, 0, ST_MIN_VAR_ADDR, NULL };

  if (argc != 2) {
    fprintf(stderr, "Usage: ./assembler foo.asm\n");
    exit(1);
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror(argv[1]);
    exit(1);
  }

  size = calc_file_size(fp);
  buf = (char *) malloc(size);
  if (buf == NULL) {
    fclose(fp);
    fprintf(stderr, "Could not allocate memory as buffer for asm file\n");
    exit(1);
  }

  n = fread(buf, sizeof(char), size, fp);
  if (n != size) {
    fclose(fp);
    free(buf);
    fprintf(stderr, "Failed to read for asm file\n");
    exit(1);
  }

  fclose(fp);

  size = strip_code(buf, size);
  n = count_orders(buf, size);
  bin = (char *) calloc(n, WORD_SIZE);
  if (bin == NULL) {
    free(buf);
    fprintf(stderr, "Could not allocate memory as buffer for hack file\n");
    exit(1);
  }

  add_reserved_symbols(&st);
  size = parse_code(buf, size, bin, &st);
  fwrite(bin, sizeof(char), size, stdout);
  st_print(&st);

  st_free(&st);
  free(bin);
  // FIXME: double free or corruption
  // free(buf);

  exit(0);
}
