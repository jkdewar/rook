#include "lex.h"
#include "parse.h"
#include "allocator/linear_allocator.h"
#include "allocator/std_heap_allocator.h"
#include "allocator/general_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
void test() {
    uint8_t memory[200];
    general_allocator_data_t general_allocator_data;
    allocator_t allocator;
#define NUM_POINTERS 3
    int *pointers[NUM_POINTERS];
    int i;

    /* create allocator */
    general_allocator_init(&general_allocator_data, memory, sizeof(memory) / sizeof(memory[0]));
    allocator.user_data = &general_allocator_data;
    allocator.alloc_fn = general_allocator_alloc;
    allocator.free_fn = general_allocator_free;

    for (i = 0; i < NUM_POINTERS; ++i) {
        pointers[i] = allocator.alloc_fn(allocator.user_data, sizeof(int));
        *pointers[i] = i;
    }
    for (i = 0; i < NUM_POINTERS; ++i) {
        printf("pointers[%d] = %p, *pointers[%d] = %d\n", i, pointers[i], i, *pointers[i]);
    }

/*
    for (i = 0; i < 1000; ++i) {
        int r = rand() % NUM_POINTERS;
        allocator.free_fn(allocator.user_data, pointers[r]);
        pointers[r] = NULL;
    }
*/
#if 0
    for (i = 0; i < NUM_POINTERS - 1; ++i)
        allocator.free_fn(allocator.user_data, pointers[i]);
#endif
    allocator.free_fn(allocator.user_data, pointers[0]);
    allocator.free_fn(allocator.user_data, pointers[1]);
    general_allocator_debug_dump(&general_allocator_data);
    printf("\n");
    allocator.free_fn(allocator.user_data, pointers[2]);
    general_allocator_debug_dump(&general_allocator_data);
    printf("\n");
}

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    test();
#if 0
    lex_input_t lex_in;
    lex_output_t lex_out;
    const char *s = 
"// this is a test\n"
"var x \"hello\"\n";
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
                printf("(TK_INT_LITERAL) %d", token->data.i); 
                break;
            case TK_FLOAT_LITERAL:      
                printf("(TK_FLOAT_LITERAL) %f", token->data.f); 
                break;
            case TK_DOUBLE_LITERAL:     
                printf("(TK_DOUBLE_LITERAL) %f", token->data.d); 
                break;
            case TK_IDENTIFIER:         
                printf("(TK_IDENTIFIER) %s", token->data.s); 
                break;
            case TK_STRING_LITERAL:     
                printf("(TK_STRING_LITERAL) \"%s\"", token->data.s); 
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
#endif
    return 0;
}
