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
"42 + 13";
    int i;
    token_t *token;
    parse_input_t parse_in;
    parse_output_t parse_out;

    uint8_t memory[1024 * 16];
    general_allocator_data_t general_allocator_data;
    allocator_t allocator;

    /* create allocator */
    general_allocator_init(&general_allocator_data, memory, sizeof(memory) / sizeof(memory[0]));
    allocator.user_data = &general_allocator_data;
    allocator.alloc_fn = general_allocator_alloc;
    allocator.free_fn = general_allocator_free;

    /* lex */
    lex_in.s = s;
    lex_in.allocator = allocator;
    lex(&lex_in, &lex_out);

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
            case TK_PLUS:
                printf("TK_PLUS");
                break;
            case TK_MINUS:
                printf("TK_MINUS");
                break;
            case TK_STAR:
                printf("TK_STAR");
                break;
            case TK_SLASH:
                printf("TK_SLASH");
                break;
        }
        printf("\n");
    }

    /* parse */
    parse_in.lex_out = &lex_out;
    parse_in.allocator = std_heap_allocator;
    parse(&parse_in, &parse_out);

    /* clean up */
    lex_output_free(&lex_out);

    return 0;
}
