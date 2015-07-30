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

#define ALLOC(SIZE) l->in->allocator->alloc_fn(l->in->allocator, SIZE)

static void error(lex_state_t *l);
static int next_token(lex_state_t *l);
static int is_whitespace(char c);
static int is_decimal_digit(char c);
static int is_hexadecimal_digit(char c);
static int is_identifier_char(char c);
static void skip_whitespace(lex_state_t *l);
static void skip_comment(lex_state_t *l);
static void skip_comments_and_whitespace(lex_state_t *l);
static int match_keyword(token_t *token);
static void read_identifier(lex_state_t *l, token_t *token);
static void read_number(lex_state_t *l, token_t *token);
static void read_string(lex_state_t *l, token_t *token);

/*----------------------------------------------------------------------*/
void lex(lex_input_t *in, lex_output_t *out) {
    lex_state_t lex_state;
    lex_state_t *l = &lex_state;

    l->in = in;
    l->out = out;
    l->out->s = l->in->s;
    l->ptr = l->in->s;
    l->token_alloced = 1024; /* default reserved size for tokens */
    /* TODO:jkd token list currently does not use allocator, because we use realloc */
    /* TODO:jkd token list also leaks */
    l->out->tokens = (token_t*)malloc(l->token_alloced * sizeof(token_t));
    l->out->token_count = 0;
    l->out->is_error = 0;
    l->out->error_string[0] = '\0';

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
void lex_token_pos(lex_output_t *out, size_t token_index, token_pos_t *token_pos) {
    token_t *token;
    const char *p = out->s;
    const char *token_start;

    if (out->token_count == 0) {
        token_pos->line_start = out->s;
        token_pos->line_end = out->s;
        token_pos->line_num = 0;
        token_pos->line_pos = 0;
        return;
    }

    if (token_index >= out->token_count) {
        token = &out->tokens[out->token_count - 1];
    } else {
        token = &out->tokens[token_index];
    }
    token_start = out->s + token->source_pos;
    token_pos->line_num = 1;
    token_pos->line_start = p;
    while (p < token_start) {
        if (*p == '\n') {
            token_pos->line_num += 1;
            token_pos->line_start = p + 1;
        }
        p += 1;
    }
    token_pos->line_end = token_pos->line_start;
    while (*token_pos->line_end != '\0' &&
           *token_pos->line_end != '\n') {
        token_pos->line_end += 1;
    }
    token_pos->line_pos = token->source_pos - (token_pos->line_start - out->s);
}

/*----------------------------------------------------------------------*/
static void error(lex_state_t *l) {
    const char *p;
    size_t line_num;
    const char *line_start;
    const char *line_end;
    char buf[1024]; /* TODO:jkd */
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

    printf("test.bas:%ld:%ld: lex error\n", line_num, l->ptr - line_start + 1);
    memcpy(buf, line_start, line_end - line_start);
    buf[line_end - line_start] = '\0';
    printf("%s\n", buf);
    for (i = 0; i < l->ptr - line_start; ++i)
        printf(" ");
    printf("^\n");

    l->out->is_error = 1;

    longjmp(l->jmpbuf, 1);
}

/*----------------------------------------------------------------------*/
static int next_token(lex_state_t *l) {
    token_t *token;
    char c;
    char cn;
    token = &l->out->tokens[l->out->token_count];
    skip_comments_and_whitespace(l);
    token->source_pos = l->ptr - l->in->s;
    c  = *(l->ptr + 0);
    cn = *(l->ptr + 1);
    if (c == '\0') {
        return 0;
    } else if (c == '\"') {
        read_string(l, token);
    } else if (is_decimal_digit(c)) {
        read_number(l, token);
    } else if (is_identifier_char(c)) {
        read_identifier(l, token);
        match_keyword(token);
    } else if (c == '<' && cn == '=') {
        token->type = TK_LESS_EQUAL;
        l->ptr += 2;
    } else if (c == '>' && cn == '=') {
        token->type = TK_GREATER_EQUAL;
        l->ptr += 2;
    } else if (c == '+') {
        token->type = TK_PLUS;
        ++l->ptr;
    } else if (c == '-') {
        token->type = TK_MINUS;
        ++l->ptr;
    } else if (c == '*') {
        token->type = TK_STAR;
        ++l->ptr;
    } else if (c == '/') {
        token->type = TK_SLASH;
        ++l->ptr;
    } else if (c == '=') {
        token->type = TK_EQUALS;
        ++l->ptr;
    } else if (c == '<') {
        token->type = TK_LESS;
        ++l->ptr;
    } else if (c == '>') {
        token->type = TK_GREATER;
        ++l->ptr;
    } else if (c == '(') {
        token->type = TK_LBRACKET;
        ++l->ptr;
    } else if (c == ')') {
        token->type = TK_RBRACKET;
        ++l->ptr;
    } else if (c == ',') {
        token->type = TK_COMMA;
        ++l->ptr;
    } else if (c == ':') {
        token->type = TK_COLON;
        ++l->ptr;
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
static int match_keyword(token_t *token) {
    static struct { char *str; token_type_t type; } keywords[] = {
        { "end", TK_END },
        { "var", TK_VAR },
        { "func", TK_FUNCTION },
        { "if", TK_IF },
        { "else", TK_ELSE },
        { "for", TK_FOR },
        { "return", TK_RETURN },
        { "and", TK_AND },
        { "or", TK_OR }
    };
    static size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);
    size_t i;

    if (token->type != TK_IDENTIFIER)
        return 0;
    for (i = 0; i < keyword_count; ++i) {
        if (strcmp(token->u.s, keywords[i].str) != 0)
            continue;
        token->type = keywords[i].type;
        return 1;
    }
    return 0;
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
    token->u.s = (char *)ALLOC(len + 1);
    memcpy(token->u.s, start, len);
    token->u.s[len] = '\0';
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
            token->u.f = (float)i + (float)after_dot / (float)after_dot_divisor;
        } else {
            token->type = TK_DOUBLE_LITERAL;
            token->u.d = (double)i + (double)after_dot / (double)after_dot_divisor;
        }
    } else {
        token->type = TK_INT_LITERAL;
        token->u.i = i;
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
    token->u.s = (char *)ALLOC(len + 1);
    memcpy(token->u.s, start, len);
    token->u.s[len] = '\0';
}
