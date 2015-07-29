#include "lex.h"
#include "parse.h"
#include "compile.h"
#include "vm.h"
#include "allocator/linear_allocator.h"
#include "allocator/std_heap_allocator.h"
#include "allocator/general_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
static int test_compiler() {

    lex_input_t lex_in;
    lex_output_t lex_out;
    const char *s = 
"function main()\n"
"    return 1987 + 4\n"
"end\n";
    parse_input_t parse_in;
    parse_output_t parse_out;
    compile_input_t compile_in;
    compile_output_t compile_out;
    allocator_t allocator;
    uint8_t memory[1024*1024];
    
    /* create allocator */
    allocator = make_linear_allocator(memory, sizeof(memory)/sizeof(memory[0]));

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

    /* compile */
    compile_in.parse_out = &parse_out;
    compile_out.bytestream.allocator = allocator;
    compile(&compile_in, &compile_out);

    /* dump bytecode */
    {
        size_t i;
        size_t num_bytes = compile_out.bytestream.ptr - compile_out.bytestream.start;
        uint8_t *ptr = compile_out.bytestream.start;

        for (i = 0; i < num_bytes; ++i) {
            if (( i % 8) == 0) {
                printf("\n%04lx: ", i);
            }
            printf("%02X ", ptr[i]);
        }
        printf("\n");
    }

    /* run vm */
    {
        vm_t vm;
        vm.bytecode_stream = &compile_out.bytestream;
        vm_run(&vm);
    }

    return 0;
}

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    return test_compiler();
}
