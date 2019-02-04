#include "vm-translator.h"

int
main(int argc, char *argv[]) {
  struct vm_list vl = { NULL, 0, 0 };
  struct cmd_list cl = { NULL, 0, 0 };
  struct label lbl = { "LABEL", 0 };
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

  for (i = 0; i < cl.idx; ++i) {
#ifdef DEBUG
    buf = stringify_command(cl.cmds[i]->type);
    fprintf(stdout, "================================================\n");
    fprintf(stdout, "%-16s%-16s%-16s\n", buf, cl.cmds[i]->arg1, cl.cmds[i]->arg2);
    fprintf(stdout, "------------------------------------------------\n");
#endif
    buf = parse_vm_command(cl.cmds[i], &lbl);
    fprintf(stdout, "%s\n", buf);
  }
  cl_free(&cl);
  fprintf(stdout, "%s\n", build_finish_command(&lbl));

  exit(0);
}
