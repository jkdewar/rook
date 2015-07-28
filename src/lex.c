#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

typedef struct {
    lex_input_t *in;
    lex_output_t *out;
    const char *ptr;
    size_t token_alloced; /* number of elements allocated in out->tokens */
    jmp_buf jmpbuf; /* used for error handling */
} lex_state_t;

#define ALLOC(SIZE) l->in->allocator.alloc_fn(l->in->allocator.user_data, SIZE)

static void error(lex_state_t *l);
static int next_token(lex_state_t *l);
static int is_whitespace(char c);
static int is_decimal_digit(char c);
static int is_hexadecimal_digit(char c);
static int is_identifier_char(char c);
static void skip_whitespace(lex_state_t *l);
static void skip_comment(lex_state_t *l);
static void skip_comments_and_whitespace(lex_state_t *l);
static void read_identifier(lex_state_t *l, token_t *token);
static void read_number(lex_state_t *l, token_t *token);
static void read_string(lex_state_t *l, token_t *token);

/*----------------------------------------------------------------------*/
void lex(lex_input_t *in, lex_output_t *out) {
    lex_state_t lex_state;
    lex_state_t *l = &lex_state;

    l->in = in;
    l->out = out;
    l->ptr = l->in->s;
    l->token_alloced = 1024; /* default reserved size for tokens */
    /* TODO:jkd token list currently does not use allocator, because we use realloc */
    l->out->tokens = (token_t*)malloc(l->token_alloced * sizeof(token_t));
    l->out->token_count = 0;
    l->out->error[0] = '\0';
    
    if (setjmp(l->jmpbuf)) {
        return;
    }

    for (;;) {
        /* allocate more space for tokens, if needed */
        if (l->out->token_count >= l->token_alloced) {
            l->token_alloced *= 2;
            l->out->tokens = (token_t*)realloc(
                    l->out->tokens,
                    l->token_alloced * sizeof(token_t));
        }
        /* read the next token */
        if (!next_token(l))
            break;
        l->out->token_count += 1;
    }
}

/*----------------------------------------------------------------------*/
void lex_output_free(lex_output_t *out) {
/*
    size_t i;
    for (i = 0; i < out->token_count; ++i) {
        token_t *token = &out->tokens[i];
        if (token->type == TK_IDENTIFIER ||
            token->type == TK_STRING_LITERAL) {
            free(token->data.s);
         }
    }
*/
    free(out->tokens);
    out->tokens = NULL;
    out->token_count = 0;
}

/*----------------------------------------------------------------------*/
const char *token_type_str(token_type_t token_type) {
    switch (token_type) {
        case TK_IDENTIFIER:     return "TK_IDENTIFIER";
        case TK_INT_LITERAL:    return "TK_INT_LITERAL";
        case TK_FLOAT_LITERAL:  return "TK_FLOAT_LITERAL";
        case TK_DOUBLE_LITERAL: return "TK_DOUBLE_LITERAL";
        case TK_STRING_LITERAL: return "TK_STRING_LITERAL";
    }
    return "TK_???";
}

/*----------------------------------------------------------------------*/
static void error(lex_state_t *l) {
    const char *p;
    int line_num;
    const char *line_start;
    const char *line_end;
    char buf[1024];
    int i;

    /* find the line on which the error occurred */
    p = l->in->s;
    line_num = 1;
    line_start = p;
    while (p < l->ptr) {
        if (*p == '\n') {
            line_num += 1;
            line_start = p + 1;
        }
        p += 1;
    }
    line_end = line_start;
    while (*line_end != '\0' && 
           *line_end != '\n') {
        line_end += 1;
    }

    printf("ERROR:%d:%ld\n", line_num, l->ptr - line_start + 1);
    memcpy(buf, line_start, line_end - line_start);
    printf("%s\n", buf);
    for (i = 0; i < l->ptr - line_start; ++i)
        printf(" ");
    printf("^\n");

    longjmp(l->jmpbuf, 1);
}

/*----------------------------------------------------------------------*/
static int next_token(lex_state_t *l) {
    token_t *token; 
    char c;
    token = &l->out->tokens[l->out->token_count];
    skip_comments_and_whitespace(l);
    c = *l->ptr;
    if (c == '\0') {
        return 0;
    } else if (c == '\"') {
        read_string(l, token);
    } else if (is_decimal_digit(c)) {
        read_number(l, token);
    } else if (is_identifier_char(c)) {
        read_identifier(l, token);
    } else {
        error(l);
    }
    return 1;    
}

/*----------------------------------------------------------------------*/
static int is_whitespace(char c) {
    switch (c) {
        case ' ':
        case '\t':
        case '\r':
        case '\n': 
            return 1;
        default:
            break;
    }
    return 0;
}

/*----------------------------------------------------------------------*/
static int is_decimal_digit(char c) {
    return c >= '0' && c <= '9'; 
}

/*----------------------------------------------------------------------*/
static int is_hexadecimal_digit(char c) {
    return  is_decimal_digit(c) ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F');
}

/*----------------------------------------------------------------------*/
static int is_identifier_char(char c) {
   return (c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '_';
}

/*----------------------------------------------------------------------*/
static void skip_whitespace(lex_state_t *l) {
    while (is_whitespace(*l->ptr)) {
        l->ptr += 1;
    }
}

/*----------------------------------------------------------------------*/
static void skip_comment(lex_state_t *l) {
    while (*(l->ptr + 0) == '/' && 
           *(l->ptr + 1) == '/') {
        l->ptr += 2;
        while (*l->ptr != '\0' && 
               *l->ptr != '\r' && 
               *l->ptr != '\n') {
            l->ptr += 1;
        }
    }
}

/*----------------------------------------------------------------------*/
static void skip_comments_and_whitespace(lex_state_t *l) {
    for (;;) {
        const char *old = l->ptr;
        skip_whitespace(l);
        skip_comment(l);
        if (old == l->ptr)
            break;
    }
}

/*----------------------------------------------------------------------*/
static void read_identifier(lex_state_t *l, token_t *token) {
    const char *start;
    const char *end;
    size_t len;
    
    /* find the end of the identifier */
    start = l->ptr;
    end = l->ptr;
    for (;;) {
        if (!is_identifier_char(*end))
            break;
        end += 1;
    }
    
    /* copy identifier string to the token */
    len = end - start;
    token->type = TK_IDENTIFIER;
    token->data.s = (char *)ALLOC(len + 1);
    memcpy(token->data.s, start, len);
    token->data.s[len] = '\0';
    l->ptr += len;
}

/*----------------------------------------------------------------------*/
static void read_number(lex_state_t *l, token_t *token) {
    int i;
    double d;
    int digit;
    int after_dot;
    int after_dot_divisor;

    /* read the left part up to the dot */
    i = 0;
    while (is_decimal_digit(*l->ptr)) {
        digit = (*l->ptr - '0');
        i *= 10; 
        i += digit;
        ++l->ptr;
    }
    if (*l->ptr == '.') {
        /* read the right part after the dot */
        d = i;
        ++l->ptr;
        after_dot = 0;
        after_dot_divisor = 1;
        while (is_decimal_digit(*l->ptr)) {
            digit = (*l->ptr - '0');
            after_dot *= 10;
            after_dot += digit;
            after_dot_divisor *= 10;
            ++l->ptr;
        }
        /* read suffix */
        if (*l->ptr == 'f') {
            ++l->ptr;
            token->type = TK_FLOAT_LITERAL;
            token->data.f = (float)i + (float)after_dot / (float)after_dot_divisor;
        } else {
            token->type = TK_DOUBLE_LITERAL;
            token->data.d = (double)i + (double)after_dot / (double)after_dot_divisor;
        }
    } else {
        token->type = TK_INT_LITERAL;
        token->data.i = i;
    }
}

/*----------------------------------------------------------------------*/
static void read_string(lex_state_t *l, token_t *token) {
    const char *start;
    const char *end;
    size_t len;
    
    /* opening quote */
    if (*l->ptr != '\"') {
        error(l);
    }
    ++l->ptr;

    /* find the closing quote */
    start = l->ptr;
    end = l->ptr;
    while (*end != '\0' && *end != '\"') {
        end += 1;
    }
    if (*end != '\"') {
        error(l);
    }
    l->ptr = end + 1;
    
    /* copy string to the token */
    len = end - start;
    token->type = TK_STRING_LITERAL;
    token->data.s = (char *)ALLOC(len + 1);
    memcpy(token->data.s, start, len);
    token->data.s[len] = '\0';
}
