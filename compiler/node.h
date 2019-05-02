#ifndef COMPILER_NODE_H_
#define COMPILER_NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_PT_CHILDREN_SIZE 4
#define MAX_NUM_LENGTH 16

enum pt_node_type {
  PTNT_ROOT,
  PTNT_KEYWORD,
  PTNT_IDENTIFIER,
  PTNT_SYMBOL,
  PTNT_LITERAL_STR,
  PTNT_LITERAL_NUM,
  PTNT_CLASS,
  PTNT_CLASS_BODY,
  PTNT_CLASS_VAR,
  PTNT_SUBROUTINE,
  PTNT_PARAMS,
  PTNT_SUBROUTINE_BODY,
  PTNT_SUBROUTINE_VAR,
  PTNT_STMTS,
  PTNT_LET_STMT,
  PTNT_IF_STMT,
  PTNT_WHILE_STMT,
  PTNT_DO_STMT,
  PTNT_RETURN_STMT,
  PTNT_SUBROUTINE_CALL,
  PTNT_ARGS,
  PTNT_EXPR,
  PTNT_PRIMARY
};

struct pt_node {
  enum pt_node_type type;
  char *value;
  struct pt_node **children;
  int size;
  int idx;
};

struct pt_node *
build_pt_root(struct pt_node *klass);

struct pt_node *
build_pt_class(char *identifiler, struct pt_node *class_body);

struct pt_node *
build_pt_class_body(void);

void
add_pt_class_var_to_class_body(struct pt_node *class_body, struct pt_node *class_var);

void
add_pt_subroutine_to_class_body(struct pt_node *class_body, struct pt_node *subroutine);

struct pt_node *
build_pt_class_var(char *scope, char *type, char *identifiler);

void
add_pt_class_var_to_class_var(struct pt_node *class_var, char *identifiler);

struct pt_node *
build_pt_subroutine(char *s_type, char *r_type, char *identifiler,
                    struct pt_node *params, struct pt_node *subroutine_body);

struct pt_node *
build_pt_param(char *type, char *identifiler);

void
add_pt_param_to_params(struct pt_node *params, char *type, char *identifiler);

struct pt_node *
build_pt_subroutine_body(void);

void
add_pt_subrt_var_to_subrt_body(struct pt_node *subrt_body, struct pt_node *subrt_var);

void
add_pt_stmt_to_subrt_body(struct pt_node *subrt_body, struct pt_node *stmt);

struct pt_node *
build_pt_subroutine_var(char *type, char *identifiler);

void
add_pt_subrt_var_to_subrt_var(struct pt_node *subrt_var, char *identifiler);

struct pt_node *
build_pt_stmts(void);

void
add_pt_stmt_to_stmts(struct pt_node *stmts, struct pt_node *stmt);

struct pt_node *
build_pt_let_stmt(char *identifiler, struct pt_node *expr);

struct pt_node *
build_pt_let_arr_stmt(char *identifiler, struct pt_node *expr1, struct pt_node *expr2);

struct pt_node *
build_pt_if_stmt(struct pt_node *expr, struct pt_node *stmts);

struct pt_node *
build_pt_if_else_stmt(struct pt_node *expr, struct pt_node *stmts1, struct pt_node *stmts2);

struct pt_node *
build_pt_while_stmt(struct pt_node *expr, struct pt_node *stmt);

struct pt_node *
build_pt_do_stmt(struct pt_node *subrt_call);

struct pt_node *
build_pt_return_stmt(struct pt_node *expr);

struct pt_node *
build_pt_subroutine_call(char *identifiler1, char *identifiler2, struct pt_node *args);

struct pt_node *
build_pt_args(void);

void
add_pt_expr_to_args(struct pt_node *args, struct pt_node *expr);

struct pt_node *
build_pt_expr(struct pt_node *primary1, char *symbol, struct pt_node *primary2);

struct pt_node *
build_pt_primary_literal_num(int num);

struct pt_node *
build_pt_primary_literal_str(char *str);

struct pt_node *
build_pt_primary_reserved(char *str);

struct pt_node *
build_pt_primary_identifier(char *identifiler);

struct pt_node *
build_pt_primary_arr(char *identifiler, struct pt_node *expr);

struct pt_node *
build_pt_primary_subrt_call(struct pt_node *subroutine_call);

struct pt_node *
build_pt_primary_parenthesis(struct pt_node *expr);

struct pt_node *
build_pt_primary_unary_operation(char *op, struct pt_node *primary);

#endif  // COMPILER_NODE_H_
