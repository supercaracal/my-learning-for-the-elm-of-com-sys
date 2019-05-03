#ifndef COMPILER_PARSE_TREE_H_
#define COMPILER_PARSE_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void print_pt_as_xml(FILE *fd, struct pt_node *node, int indent);
void free_pt(struct pt_node *node);

#endif  // COMPILER_PARSE_TREE_H_
