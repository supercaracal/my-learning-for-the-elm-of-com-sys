#include "vm-translater.h"

int
main(int argc, char *argv[]) {
  struct vm_list vl = { NULL, 0, 0 };
  struct cmd_list cl = { NULL, 0, 0 };
  struct stack *stk;
  int i;
  char *buf;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./vm-translater foo.vm\n");
    fprintf(stderr, "       ./vm-translater /path/to/dir/\n");
    exit(1);
  }

  read_vm_files(argv[1], &vl);
  lex_vm_files(&vl, &cl);
  vl_free(&vl);

  stk = stk_alloc();
  for (i = 0; i < cl.idx; ++i) {
#ifdef DEBUG
    buf = stringify_command(cl.cmds[i]->type);
    fprintf(stdout, "================================================\n");
    fprintf(stdout, "%-16s%-16s%-16s\n", buf, cl.cmds[i]->arg1, cl.cmds[i]->arg2);
    fprintf(stdout, "------------------------------------------------\n");
#endif
    buf = parse_vm_command(cl.cmds[i], stk);
    fprintf(stdout, "%s\n", buf);
  }
  cl_free(&cl);
  stk_free(stk);

  exit(0);
}
