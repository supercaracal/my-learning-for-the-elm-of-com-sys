#include "traverser.h"

#define MIN_PATH_LIST_SIZE 16

static void
pl_alloc(struct path_list *pl);

static void
pl_add(struct path_list *pl, char *buf);

void
traverse(const char *root, struct path_list *pl) {
  DIR *d;
  struct dirent *e;
  int ulen;
  int tlen;
  char *buf;

  d = opendir(root);
  if (d == NULL) {
    perror(root);
    return;
  }

  while ((e = readdir(d)) != NULL) {
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;

    ulen = strlen(root);
    tlen = ulen + strlen(e->d_name);
    if (root[ulen - 1] != '/') ++tlen;

    buf = (char *) malloc(++tlen);  // inclement for null terminator
    if (buf == NULL) {
      fprintf(stderr, "Could not allocate memory for traversing path.\n");
      break;
    }

    strcpy(buf, root);
    if (root[ulen - 1] != '/') strcat(buf, "/");
    strcat(buf, e->d_name);

    switch (e->d_type) {
      case DT_DIR:
        traverse(buf, pl);
        free(buf);
        break;
      case DT_REG:
        pl_add(pl, buf);
        break;
    }
  }

  closedir(d);
}

static void
pl_alloc(struct path_list *pl) {
  char **p;

  if (pl->paths == NULL) {
    pl->size = MIN_PATH_LIST_SIZE;
    pl->idx = 0;
    pl->paths = (char **) calloc(MIN_PATH_LIST_SIZE, sizeof(char *));
    if (pl->paths == NULL) fprintf(stderr, "Could not allocate memory for traversing paths\n");
  } else if (pl->idx >= pl->size) {
    pl->size *= 2;
    p = (char **) realloc(pl->paths, pl->size * sizeof(char *));
    if (p == NULL) {
      fprintf(stderr, "Could not reallocate memory for traversing paths\n");
    } else {
      pl->paths = p;
    }
  }
}

static void
pl_add(struct path_list *pl, char *buf) {
  pl_alloc(pl);
  pl->paths[pl->idx++] = buf;
}
