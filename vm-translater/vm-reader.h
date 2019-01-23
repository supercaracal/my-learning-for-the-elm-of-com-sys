#ifndef VM_TRANSLATER_VM_READER_H_
#define VM_TRANSLATER_VM_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "data_types.h"
#include "file-reader.h"
#include "traverser.h"
#include "vm-reader.h"

void read_vm_files(const char *path, struct vm_list *vl);
void vl_free(struct vm_list *vl);

#endif  // VM_TRANSLATER_VM_READER_H_
