#pragma once
#include "types.h"

typedef enum {
    AST_STATEMENT_DECLARE_VARIABLE,
    AST_STATEMENT_DEFINE_FUNCTION,
    AST_STATEMENT_RETURN,
    AST_STATEMENT_IF,
    AST_STATEMENT_FOR,
    AST_STATEMENT_ASSIGNMENT
} ast_statement_tag_t;

typedef struct ast_expression_t ast_expression_t;
typedef struct ast_expression_list_t ast_expression_list_t;
typedef struct ast_statement_t ast_statement_t;

typedef struct {
    token_t *name_token;
    token_t *type_token;
} ast_statement_declare_variable_t;

typedef struct ast_function_parameter_t {
    token_t *identifier_token;
    token_t *type_token;
    struct ast_function_parameter_t *next;
} ast_function_parameter_t;

typedef struct {
    token_t *name_token;
    ast_function_parameter_t *first_parameter;
    token_t *return_type_token;
    ast_statement_t *block;
} ast_statement_define_function_t;

typedef struct {
    ast_expression_t *expr;
} ast_statement_return_t;

typedef struct {
    ast_expression_t *condition;
    ast_statement_t *if_block;
    ast_statement_t *else_block;
} ast_statement_if_t;

typedef struct {
    ast_statement_t *initialize;
    ast_expression_t *condition;
    ast_statement_t *increment;
    ast_statement_t *block;
} ast_statement_for_t;

typedef struct {
    token_t *identifier;
    ast_expression_t *expr;
} ast_statement_assignment_t;

typedef union {
    ast_statement_declare_variable_t declare_variable;
    ast_statement_define_function_t define_function;
    ast_statement_return_t return_;
    ast_statement_if_t if_;
    ast_statement_for_t for_;
    ast_statement_assignment_t assignment;
} ast_statement_union_t;

struct ast_statement_t {
    ast_statement_tag_t tag;
    ast_statement_union_t u;
    ast_statement_t *next;
};

typedef enum {
    AST_EXPRESSION_LITERAL,
    AST_EXPRESSION_VARIABLE,
    AST_EXPRESSION_FUNCTION_CALL,
    AST_EXPRESSION_BIN_OP
} ast_expression_tag_t;

typedef struct {
    token_t *token;
} ast_expression_literal_t;

typedef struct {
    token_t *token;
} ast_expression_variable_t;

typedef struct {
    token_t *identifier;
    ast_expression_list_t *parameter_expr_list;
    size_t parameter_count;
} ast_expression_function_call_t;

typedef struct {
    ast_expression_t *left;
    ast_expression_t *right;
    token_type_t operation;
} ast_expression_bin_op_t;

typedef union {
    ast_expression_literal_t literal;
    ast_expression_variable_t variable;
    ast_expression_function_call_t function_call;
    ast_expression_bin_op_t bin_op;
} ast_expression_union_t;

struct ast_expression_t {
    ast_expression_tag_t tag;
    ast_expression_union_t u;
    type_t *type;
};

struct ast_expression_list_t {
    ast_expression_t *expr;
    struct ast_expression_list_t *next;
};
