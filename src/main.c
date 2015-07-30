#include "filesys.h"
#include "lex.h"
#include "parse.h"
#include "bytecode/compile.h"
#include "bytecode/disasm.h"
#include "bytecode/vm.h"
#include "bytecode/stack.h"
#include "ast.h"
#include "allocator/linear_allocator.h"
#include "allocator/std_heap_allocator.h"
#include "allocator/general_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    lex_input_t lex_in;
    lex_output_t lex_out;
    char *source;
    parse_input_t parse_in;
    parse_output_t parse_out;
    compile_input_t compile_in;
    compile_output_t compile_out;
    uint8_t memory[1024*1024];
    allocator_t allocator;

    allocator = make_linear_allocator(memory, sizeof(memory)/sizeof(memory[0]));

    /* read source file */
    if (argc != 2) {
        printf("usage: basic <file.bas>\n");
        return -1;
    }
    if (!read_text_file(argv[1], &source)) {
        printf("error reading input file '%s'\n", argv[1]);
        return -1;
    }

    /* lex */
    lex_in.s = source;
    lex_in.allocator = &allocator;
    lex(&lex_in, &lex_out);
    if (lex_out.is_error)
        return -1;

    /* parse */
    parse_in.lex_out = &lex_out;
    parse_in.allocator = &allocator;
    parse(&parse_in, &parse_out);
    if (parse_out.is_error)
        return -1;

    /* compile */
    compile_in.parse_out = &parse_out;
    compile_out.bytestream.allocator = &allocator;
    compile(&compile_in, &compile_out);
    if (compile_out.is_error)
        return -1;

    /* dump bytecode */
    {
        size_t i;
        size_t num_bytes = compile_out.bytestream.ptr - compile_out.bytestream.start;
        uint8_t *ptr = compile_out.bytestream.start;

        for (i = 0; i < num_bytes; ++i) {
            if (( i % 16) == 0) {
                printf("\n%04lx: ", i);
            }
            printf("%02X ", ptr[i]);
        }
        printf("\n\n");
    }

    /* disassemble */
    {
        size_t size = compile_out.bytestream.ptr - compile_out.bytestream.start;
        disasm(compile_out.bytestream.start, size);
    }

/*
    printf("linear allocator used %ld bytes\n", allocator.used);
*/

    /* run vm */
    {
        vm_t vm;
        int32_t result;

        vm.bytecode = compile_out.bytestream.start;
        vm.bytecode_size = compile_out.bytestream.ptr - compile_out.bytestream.start;
        vm.stack = ALLOCATOR_ALLOC(&allocator, 1024 * 16);
        vm.ip = 0;
        vm.sp = 0;
        stack_push_ui32(&vm, 0); /* return value */
        stack_push_ui32(&vm, ~0); /* return address */
        vm.bp = vm.sp;
        vm_run(&vm);

        result = stack_pop_si32(&vm);
        printf("result: %d\n", result);
    }

    return 0;
}
