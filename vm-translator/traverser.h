#ifndef VM_TRANSLATOR_TRAVERSER_H_
#define VM_TRANSLATOR_TRAVERSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "data_types.h"

void traverse(const char *root, struct path_list *pl);

#endif  // VM_TRANSLATOR_TRAVERSER_H_
