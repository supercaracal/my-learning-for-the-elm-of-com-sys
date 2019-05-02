#include "parse_tree.h"

#define PRINT_INDENT(indent) for (int i = 0; i < indent * 2; ++i) fprintf(fd, "%c", ' ');

static void
print_pt_node_as_xml_tag(FILE *fd, struct pt_node *node, int indent, char *tag);

static void
print_term_as_tag(FILE *fd, char *value, int indent, char *tag);

static void
print_sanitized_term_as_tag(FILE *fd, char *value, int indent, char *tag);

void print_pt_as_xml(FILE *fd, struct pt_node *node, int indent) {
  if (node == NULL) return;

  switch (node->type) {
    case PTNT_ROOT:
      print_pt_node_as_xml_tag(fd, node, indent, NULL);
      break;
    case PTNT_KEYWORD:
      print_term_as_tag(fd, node->value, indent, "keyword");
      break;
    case PTNT_IDENTIFIER:
      print_term_as_tag(fd, node->value, indent, "identifier");
      break;
    case PTNT_SYMBOL:
      print_sanitized_term_as_tag(fd, node->value, indent, "symbol");
      break;
    case PTNT_LITERAL_STR:
      print_term_as_tag(fd, node->value, indent, "stringConstant");
      break;
    case PTNT_LITERAL_NUM:
      print_term_as_tag(fd, node->value, indent, "integerConstant");
      break;
    case PTNT_CLASS:
      print_pt_node_as_xml_tag(fd, node, indent, "class");
      break;
    case PTNT_CLASS_BODY:
      print_pt_node_as_xml_tag(fd, node, indent, NULL);
      break;
    case PTNT_CLASS_VAR:
      print_pt_node_as_xml_tag(fd, node, indent, "classVarDec");
      break;
    case PTNT_SUBROUTINE:
      print_pt_node_as_xml_tag(fd, node, indent, "subroutineDec");
      break;
    case PTNT_PARAMS:
      print_pt_node_as_xml_tag(fd, node, indent, "parameterList");
      break;
    case PTNT_SUBROUTINE_BODY:
      print_pt_node_as_xml_tag(fd, node, indent, "subroutineBody");
      break;
    case PTNT_SUBROUTINE_VAR:
      print_pt_node_as_xml_tag(fd, node, indent, "varDec");
      break;
    case PTNT_STMTS:
      print_pt_node_as_xml_tag(fd, node, indent, "statements");
      break;
    case PTNT_LET_STMT:
      print_pt_node_as_xml_tag(fd, node, indent, "letStatement");
      break;
    case PTNT_IF_STMT:
      print_pt_node_as_xml_tag(fd, node, indent, "ifStatement");
      break;
    case PTNT_WHILE_STMT:
      print_pt_node_as_xml_tag(fd, node, indent, "whileStatement");
      break;
    case PTNT_DO_STMT:
      print_pt_node_as_xml_tag(fd, node, indent, "doStatement");
      break;
    case PTNT_RETURN_STMT:
      print_pt_node_as_xml_tag(fd, node, indent, "returnStatement");
      break;
    case PTNT_SUBROUTINE_CALL:
      print_pt_node_as_xml_tag(fd, node, indent, NULL);
      break;
    case PTNT_ARGS:
      print_pt_node_as_xml_tag(fd, node, indent, "expressionList");
      break;
    case PTNT_EXPR:
      print_pt_node_as_xml_tag(fd, node, indent, "expression");
      break;
    case PTNT_PRIMARY:
      print_pt_node_as_xml_tag(fd, node, indent, "term");
      break;
    default:
      break;
  }
}

static void
print_pt_node_as_xml_tag(FILE *fd, struct pt_node *node, int indent, char *tag) {
  int i;

  if (tag != NULL) {
    PRINT_INDENT(indent);
    fprintf(fd, "<%s>\n", tag);
  }

  for (i = 0; i < node->size; ++i) {
    print_pt_as_xml(fd, node->children[i], tag == NULL ? indent : indent + 1);
  }

  if (tag != NULL) {
    PRINT_INDENT(indent);
    fprintf(fd, "</%s>\n", tag);
  }
}

static void
print_term_as_tag(FILE *fd, char *value, int indent, char *tag) {
  PRINT_INDENT(indent);
  fprintf(fd, "<%s> %s </%s>\n", tag, value, tag);
}

static void
print_sanitized_term_as_tag(FILE *fd, char *value, int indent, char *tag) {
  if (strcmp(value, "&") == 0) {
    value = "&amp;";
  } else if (strcmp(value, "<") == 0) {
    value = "&lt;";
  } else if (strcmp(value, ">") == 0) {
    value = "&gt;";
  }
  print_term_as_tag(fd, value, indent, tag);
}
