#pragma once

typedef enum {
    AST_STATEMENT_DECLARE_VARIABLE,
    AST_STATEMENT_ASSIGN,
    AST_STATEMENT_IF,
    AST_STATEMENT_FOR
} ast_statement_type_t;

typedef enum {
    AST_EXPRESSION_WHATEVER
} ast_expression_type_t;

typedef struct ast_statement_t {
    ast_statement_type_t type;
    void *type_specific;
    struct ast_statement_t *next;
} ast_statement_t;

typedef struct {
    char *name;
} ast_statement_declare_variable_t;

typedef struct {
    ast_expression_type_t type;
    void *type_specific;
} ast_expression_t;
