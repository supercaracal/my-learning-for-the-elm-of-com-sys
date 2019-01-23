#include "vm-reader.h"

#define MIN_VM_LIST_SIZE 16

static void vl_alloc(struct vm_list *vl);
static void vl_add(struct vm_list *vl, char *buf);
static int is_vm_file(const char *path);

void
read_vm_files(const char *path, struct vm_list *vl) {
  struct stat statbuf;
  struct path_list pl = { NULL, 0, 0 };
  int i;

  if (stat(path, &statbuf) != 0) {
    perror(path);
    return;
  }

  switch (statbuf.st_mode & S_IFMT) {
    case S_IFDIR:
      traverse(path, &pl);
      for (i = 0; i < pl.idx; ++i) {
        if (!is_vm_file(pl.paths[i])) continue;
        vl_add(vl, read_file(pl.paths[i]));
        free(pl.paths[i]);
      }
      break;
    case S_IFREG:
      if (is_vm_file(path)) vl_add(vl, read_file(path));
      break;
  }
}

void
vl_free(struct vm_list *vl) {
  int i;

  for (i = 0; i < vl->idx; ++i) {
    free(vl->bufs[i]);
    vl->bufs[i] = NULL;
  }

  free(vl->bufs);
  vl->bufs = NULL;
}

static void
vl_alloc(struct vm_list *vl) {
  char **p;

  if (vl->bufs == NULL) {
    vl->size = MIN_VM_LIST_SIZE;
    vl->idx = 0;
    vl->bufs = (char **) calloc(MIN_VM_LIST_SIZE, sizeof(char *));
    if (vl->bufs == NULL) fprintf(stderr, "Could not allocate memory for reading vm files\n");
  } else if (vl->idx >= vl->size) {
    vl->size *= 2;
    p = (char **) realloc(vl->bufs, vl->size * sizeof(char *));
    if (p == NULL) {
      fprintf(stderr, "Could not reallocate memory for reading vm files\n");
    } else {
      vl->bufs = p;
    }
  }
}

static void
vl_add(struct vm_list *vl, char *buf) {
  vl_alloc(vl);
  vl->bufs[vl->idx++] = buf;
}

static int
is_vm_file(const char *path) {
  int len;

  len = strlen(path);

  if (len > 3 && path[len - 1] == 'm' && path[len - 2] == 'v' && path[len - 3] == '.') {
    return 1;
  } else {
    return 0;
  }
}
