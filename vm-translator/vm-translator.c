#include "vm-translator.h"

int
main(int argc, char *argv[]) {
  struct vm_list vl = { NULL, 0, 0 };
  struct cmd_list cl = { NULL, 0, 0 };
  struct label lbl = { "LABEL", 0 };
  int i;
  int bootstrap_flag;
  char *buf;
  char cfunc[MAX_CUR_FNC_NAME_SIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage: ./vm-translater foo.vm\n");
    fprintf(stderr, "       ./vm-translater /path/to/dir/\n");
    exit(1);
  }

  read_vm_files(argv[1], &vl);
  lex_vm_files(&vl, &cl);
  vl_free(&vl);

  cfunc[0] = '\0';
  bootstrap_flag = 0;
  for (i = 0; i < cl.idx; ++i) {
    if (cl.cmds[i]->type == FUNCTION && strcmp(cl.cmds[i]->arg1, INIT_FUNC_NAME) == 0) {
      bootstrap_flag = 1;
      break;
    }
  }
  if (bootstrap_flag) fprintf(stdout, "%s\n", build_bootstrap_asm_code(&lbl));
  for (i = 0; i < cl.idx; ++i) {
#ifdef DEBUG
    buf = stringify_command(cl.cmds[i]->type);
    fprintf(stdout, "================================================\n");
    fprintf(stdout, "%-16s%-16s%-16s\n", buf, cl.cmds[i]->arg1, cl.cmds[i]->arg2);
    fprintf(stdout, "------------------------------------------------\n");
#endif
    buf = parse_vm_command(cl.cmds[i], &lbl, cfunc);
    fprintf(stdout, "%s\n", buf);
  }
  cl_free(&cl);

  exit(0);
}
