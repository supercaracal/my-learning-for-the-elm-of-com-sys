#include "node.h"

static struct pt_node *build_pt_node(enum pt_node_type type, char *value);
static void add_child_to_pt(struct pt_node *parent, struct pt_node *child);
static enum pt_node_type get_type_kind(char *type);

struct pt_node *
build_pt_root(struct pt_node *klass) {
  struct pt_node *root;
  root = build_pt_node(PTNT_ROOT, NULL);
  root->children = calloc(1, sizeof(struct pt_node));
  root->size = 1;
  root->idx = 0;
  root->children[0] = klass;
  return root;
}

struct pt_node *
build_pt_class(char *identifiler, struct pt_node *class_body) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_CLASS, NULL);
  child = build_pt_node(PTNT_KEYWORD, "class");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "{");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, class_body);
  child = build_pt_node(PTNT_SYMBOL, "}");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_class_body(void) {
  return build_pt_node(PTNT_CLASS_BODY, NULL);
}

void
add_pt_class_var_to_class_body(struct pt_node *class_body, struct pt_node *class_var) {
  struct pt_node *delimiter;
  add_child_to_pt(class_body, class_var);
  delimiter = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(class_var, delimiter);
}

void
add_pt_subroutine_to_class_body(struct pt_node *class_body, struct pt_node *subroutine) {
  add_child_to_pt(class_body, subroutine);
}

struct pt_node *
build_pt_class_var(char *scope, char *type, char *identifiler) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_CLASS_VAR, NULL);
  child = build_pt_node(PTNT_KEYWORD, scope);
  add_child_to_pt(parent, child);
  child = build_pt_node(get_type_kind(type), type);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  return parent;
}

void
add_pt_class_var_to_class_var(struct pt_node *class_var, char *identifiler) {
  struct pt_node *child;
  child = build_pt_node(PTNT_SYMBOL, ",");
  add_child_to_pt(class_var, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(class_var, child);
}

struct pt_node *
build_pt_subroutine(char *s_type, char *r_type, char *identifiler,
                    struct pt_node *params, struct pt_node *subroutine_body) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_SUBROUTINE, NULL);
  child = build_pt_node(PTNT_KEYWORD, s_type);
  add_child_to_pt(parent, child);
  child = build_pt_node(get_type_kind(r_type), r_type);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "(");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, params);
  child = build_pt_node(PTNT_SYMBOL, ")");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, subroutine_body);
  child = build_pt_node(PTNT_SYMBOL, "}");
  add_child_to_pt(subroutine_body, child);
  return parent;
}

struct pt_node *
build_pt_param(char *type, char *identifiler) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PARAMS, NULL);
  if (type == NULL || identifiler == NULL) return parent;
  child = build_pt_node(get_type_kind(type), type);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  return parent;
}

void
add_pt_param_to_params(struct pt_node *params, char *type, char *identifiler) {
  struct pt_node *child;
  child = build_pt_node(PTNT_SYMBOL, ",");
  add_child_to_pt(params, child);
  child = build_pt_node(get_type_kind(type), type);
  add_child_to_pt(params, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(params, child);
}

struct pt_node *
build_pt_subroutine_body(void) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_SUBROUTINE_BODY, NULL);
  child = build_pt_node(PTNT_SYMBOL, "{");
  add_child_to_pt(parent, child);
  return parent;
}

void
add_pt_subrt_var_to_subrt_body(struct pt_node *subrt_body, struct pt_node *subrt_var) {
  struct pt_node *delimiter;
  add_child_to_pt(subrt_body, subrt_var);
  delimiter = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(subrt_var, delimiter);
}

void
add_pt_stmt_to_subrt_body(struct pt_node *subrt_body, struct pt_node *stmt) {
  add_child_to_pt(subrt_body, stmt);
}

struct pt_node *
build_pt_subroutine_var(char *type, char *identifiler) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_SUBROUTINE_VAR, NULL);
  child = build_pt_node(PTNT_KEYWORD, "var");
  add_child_to_pt(parent, child);
  child = build_pt_node(get_type_kind(type), type);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  return parent;
}

void
add_pt_subrt_var_to_subrt_var(struct pt_node *subrt_var, char *identifiler) {
  struct pt_node *child;
  child = build_pt_node(PTNT_SYMBOL, ",");
  add_child_to_pt(subrt_var, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(subrt_var, child);
}

struct pt_node *build_pt_stmts(void) {
  return build_pt_node(PTNT_STMTS, NULL);
}

void
add_pt_stmt_to_stmts(struct pt_node *stmts, struct pt_node *stmt) {
  add_child_to_pt(stmts, stmt);
}

struct pt_node *
build_pt_let_stmt(char *identifiler, struct pt_node *expr) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_LET_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "let");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "=");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_let_arr_stmt(char *identifiler, struct pt_node *expr1, struct pt_node *expr2) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_LET_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "let");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "[");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr1);
  child = build_pt_node(PTNT_SYMBOL, "]");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "=");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr2);
  child = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_if_stmt(struct pt_node *expr, struct pt_node *stmts) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_IF_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "if");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "(");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, ")");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "{");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, stmts);
  child = build_pt_node(PTNT_SYMBOL, "}");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_if_else_stmt(struct pt_node *expr, struct pt_node *stmts1, struct pt_node *stmts2) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_if_stmt(expr, stmts1);
  child = build_pt_node(PTNT_KEYWORD, "else");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "{");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, stmts2);
  child = build_pt_node(PTNT_SYMBOL, "}");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_while_stmt(struct pt_node *expr, struct pt_node *stmt) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_WHILE_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "while");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "(");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, ")");
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "{");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, stmt);
  child = build_pt_node(PTNT_SYMBOL, "}");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_do_stmt(struct pt_node *subrt_call) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_DO_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "do");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, subrt_call);
  child = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_return_stmt(struct pt_node *expr) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_RETURN_STMT, NULL);
  child = build_pt_node(PTNT_KEYWORD, "return");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, ";");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_subroutine_call(char *identifiler1, char *identifiler2, struct pt_node *args) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_SUBROUTINE_CALL, NULL);
  if (identifiler1 != NULL) {
    child = build_pt_node(PTNT_IDENTIFIER, identifiler1);
    add_child_to_pt(parent, child);
    child = build_pt_node(PTNT_SYMBOL, ".");
    add_child_to_pt(parent, child);
  }
  child = build_pt_node(PTNT_IDENTIFIER, identifiler2);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "(");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, args);
  child = build_pt_node(PTNT_SYMBOL, ")");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *build_pt_args(void) {
  return build_pt_node(PTNT_ARGS, NULL);
}

void
add_pt_expr_to_args(struct pt_node *args, struct pt_node *expr) {
  if (args->idx > 0) {
    struct pt_node *delimiter = build_pt_node(PTNT_SYMBOL, ",");
    add_child_to_pt(args, delimiter);
  }
  add_child_to_pt(args, expr);
}

struct pt_node *
build_pt_expr(struct pt_node *primary1, char *op, struct pt_node *primary2) {
  struct pt_node *parent;
  parent = build_pt_node(PTNT_EXPR, NULL);
  add_child_to_pt(parent, primary1);
  if (op != NULL) add_child_to_pt(parent, build_pt_node(PTNT_SYMBOL, op));
  if (primary2 != NULL) add_child_to_pt(parent, primary2);
  return parent;
}

struct pt_node *
build_pt_primary_literal_num(int num) {
  struct pt_node *parent;
  struct pt_node *child;
  char *str;
  str = (char *) calloc(1, MAX_NUM_LENGTH);
  snprintf(str, MAX_NUM_LENGTH, "%d", num);
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_LITERAL_NUM, str);
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_literal_str(char *str) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_LITERAL_STR, str);
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_reserved(char *str) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_KEYWORD, str);
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_identifier(char *identifier) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_IDENTIFIER, identifier);
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_arr(char *identifiler, struct pt_node *expr) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_IDENTIFIER, identifiler);
  add_child_to_pt(parent, child);
  child = build_pt_node(PTNT_SYMBOL, "[");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, "]");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_subrt_call(struct pt_node *subroutine_call) {
  struct pt_node *parent;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  add_child_to_pt(parent, subroutine_call);
  return parent;
}

struct pt_node *
build_pt_primary_parenthesis(struct pt_node *expr) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_SYMBOL, "(");
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, expr);
  child = build_pt_node(PTNT_SYMBOL, ")");
  add_child_to_pt(parent, child);
  return parent;
}

struct pt_node *
build_pt_primary_unary_operation(char *op, struct pt_node *primary) {
  struct pt_node *parent;
  struct pt_node *child;
  parent = build_pt_node(PTNT_PRIMARY, NULL);
  child = build_pt_node(PTNT_SYMBOL, op);
  add_child_to_pt(parent, child);
  add_child_to_pt(parent, primary);
  return parent;
}

static struct pt_node *
build_pt_node(enum pt_node_type type, char *value) {
  struct pt_node *node;
  node = (struct pt_node *) calloc(1, sizeof(struct pt_node));
  node->type = type;
  node->value = value;
  node->children = NULL;
  node->size = 0;
  node->idx = 0;
  return node;
}

static void
add_child_to_pt(struct pt_node *parent, struct pt_node *child) {
  if (child == NULL) return;
  if (parent->size == 0 || parent->children == NULL) {
    parent->children = (struct pt_node **) calloc(MIN_PT_CHILDREN_SIZE, sizeof(struct pt_node *));
    parent->size = MIN_PT_CHILDREN_SIZE;
    parent->idx = 0;
  } else if (parent->idx == parent->size - 1) {
    parent->size *= 2;
    struct pt_node **nodes = (struct pt_node **) realloc(parent->children, parent->size * sizeof(struct pt_node *));
    if (nodes != NULL) parent->children = nodes;
  }
  parent->children[parent->idx++] = child;
}

static enum pt_node_type
get_type_kind(char *type) {
  if (strcmp(type, "int") == 0
      || strcmp(type, "char") == 0
      || strcmp(type, "boolean") == 0
      || strcmp(type, "void") == 0) {
    return PTNT_KEYWORD;
  } else {
    return PTNT_IDENTIFIER;
  }
}
