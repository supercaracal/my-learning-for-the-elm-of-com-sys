#include "vm-lexer.h"

#define MIN_CMD_LIST_SIZE 16
#define MAX_TOKEN_SIZE 128

enum r_mode { COMMAND, ARG1, ARG2, COMMENT };

static void lex_vm_file(const char *buf, struct cmd_list *cl, int fid);
static enum r_mode set_token(struct command *cmd, const char token[MAX_TOKEN_SIZE], const enum r_mode mode);
static char *token_alloc(int size);
static struct command *cmd_alloc(int fid);
static void cl_alloc(struct cmd_list *cl);
static void cl_add(struct cmd_list *cl, struct command *cmd);

void
lex_vm_files(struct vm_list *vl, struct cmd_list *cl) {
  int i;

  for (i = 0; i < vl->idx; ++i) {
    lex_vm_file(vl->bufs[i], cl, i);
  }
}

void
cl_free(struct cmd_list *cl) {
  int i;

  for (i = 0; i < cl->idx; ++i) {
    free(cl->cmds[i]->arg1);
    free(cl->cmds[i]->arg2);
    free(cl->cmds[i]);
    cl->cmds[i] = NULL;
  }

  free(cl->cmds);
  cl->cmds = NULL;
}

static void
lex_vm_file(const char *buf, struct cmd_list *cl, int fid) {
  int i;
  int j;
  int k;
  int len;
  int init;
  char token[MAX_TOKEN_SIZE];
  enum r_mode mode;
  struct command *cmd;

  for (i = 0, j = 0, len = strlen(buf), init = 1, mode = COMMAND, cmd = NULL; i < len; ++i) {
    switch (buf[i]) {
      case '\r':
        break;
      case '\n':
        if (!init) set_token(cmd, token, mode);
        if (cmd != NULL) cl_add(cl, cmd);
        init = 1;
        mode = COMMAND;
        cmd = NULL;
        break;
      case ' ':
      case '\t':
        if (init || mode == COMMENT || i + 1 >= len) break;
        if (buf[i + 1] == ' ' || buf[i + 1] == '\t') break;
        init = 1;
        mode = set_token(cmd, token, mode);
        break;
      case '/':
        if (i + 1 < len && buf[i + 1] == '/') mode = COMMENT;
        break;
      default:
        if (mode == COMMENT) break;
        if (init) {
          for (k = 0; k < MAX_TOKEN_SIZE; ++k) token[k] = '\0';
          init = 0;
          j = 0;
        }
        if (cmd == NULL) cmd = cmd_alloc(fid);
        if (j >= MAX_TOKEN_SIZE) break;
        token[j++] = buf[i];
        break;
    }
  }
}

static enum r_mode
set_token(struct command *cmd, const char token[MAX_TOKEN_SIZE], const enum r_mode mode) {
  switch (mode) {
    case COMMAND:
      if (strcmp(token, "add") == 0) {
        cmd->type = ADD;
      } else if (strcmp(token, "sub") == 0) {
        cmd->type = SUB;
      } else if (strcmp(token, "neg") == 0) {
        cmd->type = NEG;
      } else if (strcmp(token, "eq") == 0) {
        cmd->type = EQ;
      } else if (strcmp(token, "gt") == 0) {
        cmd->type = GT;
      } else if (strcmp(token, "lt") == 0) {
        cmd->type = LT;
      } else if (strcmp(token, "and") == 0) {
        cmd->type = AND;
      } else if (strcmp(token, "or") == 0) {
        cmd->type = OR;
      } else if (strcmp(token, "not") == 0) {
        cmd->type = NOT;
      } else if (strcmp(token, "push") == 0) {
        cmd->type = PUSH;
      } else if (strcmp(token, "pop") == 0) {
        cmd->type = POP;
      } else if (strcmp(token, "label") == 0) {
        cmd->type = LABEL;
      } else if (strcmp(token, "goto") == 0) {
        cmd->type = GOTO;
      } else if (strcmp(token, "if-goto") == 0) {
        cmd->type = IFGOTO;
      } else if (strcmp(token, "function") == 0) {
        cmd->type = FUNCTION;
      } else if (strcmp(token, "call") == 0) {
        cmd->type = CALL;
      } else if (strcmp(token, "return") == 0) {
        cmd->type = RETURN;
      } else {
        fprintf(stderr, "Unknown command found: %s\n", token);
      }
      return ARG1;
    case ARG1:
      cmd->arg1 = token_alloc(strlen(token));
      strcpy(cmd->arg1, token);  // NOLINT
      return ARG2;
    case ARG2:
      cmd->arg2 = token_alloc(strlen(token));
      strcpy(cmd->arg2, token);  // NOLINT
      return COMMENT;
    default:
      return COMMENT;
  }
}

static char *
token_alloc(int size) {
  char *p;

  p = (char *) malloc(size + 1);  // increment for null terminater
  if (p == NULL) fprintf(stderr, "Could not allocate memory for a token\n");
  return p;
}

static struct command *
cmd_alloc(int fid) {
  struct command *cmd;

  cmd = (struct command *) malloc(sizeof(struct command));
  if (cmd == NULL) {
    fprintf(stderr, "Could not allocate memory for a command\n");
    return NULL;
  }

  cmd->type = UNKNOWN;
  cmd->arg1 = NULL;
  cmd->arg2 = NULL;
  cmd->fid = fid;

  return cmd;
}

static void
cl_alloc(struct cmd_list *cl) {
  struct command **p;

  if (cl->cmds == NULL) {
    cl->size = MIN_CMD_LIST_SIZE;
    cl->idx = 0;
    cl->cmds = (struct command **) calloc(MIN_CMD_LIST_SIZE, sizeof(struct command *));
    if (cl->cmds == NULL) fprintf(stderr, "Could not allocate memory for parsing command list\n");
  } else if (cl->idx >= cl->size) {
    cl->size *= 2;
    p = (struct command **) realloc(cl->cmds, cl->size * sizeof(struct command *));
    if (p == NULL) {
      fprintf(stderr, "Could not reallocate memory for parsing command list\n");
    } else {
      cl->cmds = p;
    }
  }
}

static void
cl_add(struct cmd_list *cl, struct command *cmd) {
  cl_alloc(cl);
  cl->cmds[cl->idx++] = cmd;
}
