#include "lex.h"
#include "parse.h"
#include "allocator/linear_allocator.h"
#include "allocator/std_heap_allocator.h"
#include "allocator/general_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    lex_input_t lex_in;
    lex_output_t lex_out;
    const char *s = 
"// this is a test\n"
"function @foo(x:int):int\n"
"    var x\n"
"    var y\n"
"end\n";

    int i;
    token_t *token;
    parse_input_t parse_in;
    parse_output_t parse_out;

    uint8_t memory[1024 * 16];
    allocator_t allocator;
#if 0
    general_allocator_data_t general_allocator_data;

    /* create allocator */
    general_allocator_init(&general_allocator_data, memory, sizeof(memory) / sizeof(memory[0]));
    allocator.user_data = &general_allocator_data;
    allocator.alloc_fn = general_allocator_alloc;
    allocator.free_fn = general_allocator_free;
#else
    linear_allocator_data_t linear_allocator_data;

    /* create allocator */
    linear_allocator_data.start = memory;
    linear_allocator_data.capacity = sizeof(memory) / sizeof(memory[0]);
    linear_allocator_data.used = 0;
    allocator.user_data = &linear_allocator_data;
    allocator.alloc_fn = linear_alloc;
    allocator.free_fn = linear_free;
#endif

    /* lex */
    lex_in.s = s;
    lex_in.allocator = allocator;
    lex(&lex_in, &lex_out);

#if 0
    /* dump lex result */
    for (i = 0; i < lex_out.token_count; ++i) {
        token = &lex_out.tokens[i];
        printf("%d: ", i);
        switch (token->type) {
            case TK_INT_LITERAL:        
                printf("(TK_INT_LITERAL) %d", token->u.i);
                break;
            case TK_FLOAT_LITERAL:      
                printf("(TK_FLOAT_LITERAL) %f", token->u.f);
                break;
            case TK_DOUBLE_LITERAL:     
                printf("(TK_DOUBLE_LITERAL) %f", token->u.d);
                break;
            case TK_IDENTIFIER:         
                printf("(TK_IDENTIFIER) %s", token->u.s);
                break;
            case TK_STRING_LITERAL:     
                printf("(TK_STRING_LITERAL) \"%s\"", token->u.s); 
                break;
#define T(TK) case TK: printf(#TK); break;
            T(TK_PLUS)
            T(TK_MINUS)
            T(TK_STAR)
            T(TK_SLASH)
            T(TK_EQUALS)
            T(TK_LESS)
            T(TK_LESS_EQUAL)
            T(TK_EQUALS_EQUALS)
            T(TK_GREATER_EQUAL)
            T(TK_GREATER)
            T(TK_LBRACKET)
            T(TK_RBRACKET)
            T(TK_COMMA)
            T(TK_COLON)
            T(TK_END)
            T(TK_VAR)
            T(TK_FUNCTION)
            T(TK_IF)
            T(TK_FOR)
            T(TK_RETURN)
#undef T
        }
        printf("\n");
    }
#endif

    if (lex_out.is_error) {
        return -1;
    }

    /* parse */
    parse_in.lex_out = &lex_out;
    parse_in.allocator = allocator;
    parse(&parse_in, &parse_out);

    return 0;
}
