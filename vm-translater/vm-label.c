#include "vm-label.h"

char *
create_label(struct label *lbl) {
  int size;
  char *buf;

  size = strlen(lbl->prefix) + sizeof(int) + 1;
  buf = (char *) malloc(size);
  if (buf == NULL) fprintf(stderr, "Could not allocate memory for label\n");
  snprintf(buf, size, "%s%d", lbl->prefix, lbl->i++);
  return buf;
}
