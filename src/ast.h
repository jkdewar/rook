#pragma once

typedef enum {
    AST_STATEMENT_DECLARE_VARIABLE,
    AST_STATEMENT_DEFINE_FUNCTION,
    AST_STATEMENT_RETURN,
    AST_STATEMENT_IF,
    AST_STATEMENT_FOR,
    AST_STATEMENT_ASSIGN
} ast_statement_type_t;

typedef struct {
    token_t token;
} ast_statement_declare_variable_t;

typedef struct ast_function_parameter_t {
    token_t identifier_token;
    token_t type_token;
    struct ast_function_parameter_t *next;
} ast_function_parameter_t;

typedef struct {
    token_t name_token;
    ast_function_parameter_t *first_parameter;
    token_t return_type_token;
    struct ast_statement_t *block;
} ast_statement_define_function_t;

typedef struct {
    struct ast_expression_t *expr;
} ast_statement_return_t;

typedef struct {
    struct ast_expression_t *condition;
    struct ast_statement_t *if_block;
    struct ast_statement_t *else_block;
} ast_statement_if_t;

typedef struct {
    struct ast_statement_t *initialize;
    struct ast_expression_t *condition;
    struct ast_statement_t *increment;
    struct ast_statement_t *block;
} ast_statement_for_t;

typedef struct {
        token_t identifier;
        struct ast_expression_t *expr;
} ast_statement_assign_t;

typedef union {
    ast_statement_declare_variable_t declare_variable;
    ast_statement_define_function_t define_function;
    ast_statement_return_t return_;
    ast_statement_if_t if_;
    ast_statement_for_t for_;
    ast_statement_assign_t assign;
} ast_statement_union_t;

typedef struct ast_statement_t {
    ast_statement_type_t type;
    ast_statement_union_t u;
    struct ast_statement_t *next;
} ast_statement_t;

typedef enum {
    AST_EXPRESSION_LITERAL,
    AST_EXPRESSION_BIN_OP
} ast_expression_type_t;

typedef struct {
    token_t token;
} ast_expression_literal_t;

typedef struct {
    struct ast_expression_t *left;
    struct ast_expression_t *right;
    token_type_t operation;
} ast_expression_bin_op_t;

typedef union {
    ast_expression_literal_t literal;
    ast_expression_bin_op_t bin_op;
} ast_expression_union_t;

typedef struct ast_expression_t {
    ast_expression_type_t type;
    ast_expression_union_t u;
} ast_expression_t;
