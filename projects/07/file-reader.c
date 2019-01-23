#include "file-reader.h"

static int calc_file_size(FILE *fp);

char *
read_file(const char *path)
{
  FILE *fp;
  int size;
  char *buf;
  int n;

  fp = fopen(path, "r");
  if (fp == NULL) {
    perror(path);
    return NULL;
  }

  size = calc_file_size(fp);
  buf = (char *) malloc(size);
  if (buf == NULL) {
    fclose(fp);
    fprintf(stderr, "Could not allocate memory for reading file: %s\n", path);
    return NULL;
  }

  n = fread(buf, sizeof(char), size, fp);
  if (n != size) fprintf(stderr, "Failed to read file: %s\n", path);
  fclose(fp);

  return buf;
}

static int
calc_file_size(FILE *fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}
