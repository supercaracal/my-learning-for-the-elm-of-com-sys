#ifndef COMPILER_PARSE_TREE_H_
#define COMPILER_PARSE_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void print_pt_as_xml(FILE *fd, struct pt_node *root, int indent);

#endif  // COMPILER_PARSE_TREE_H_
