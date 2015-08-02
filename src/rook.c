#include "rook.h"
#include "filesys.h"
#include "lex.h"
#include "parse.h"
#include "compile.h"
#include "disasm.h"
#include "vm.h"
#include "stack.h"
#include <stdio.h>

struct rook_state_t {
    allocator_t allocator;
    int error;
    char error_str[1024];
};

/*----------------------------------------------------------------------*/
rook_state_t *rook_open(allocator_t allocator) {
    rook_state_t *R = ALLOCATOR_ALLOC(&allocator, sizeof(rook_state_t));
    R->allocator = allocator;
    R->error = 0;
    R->error_str[0] = '\0';
    return R;
}

/*----------------------------------------------------------------------*/
void rook_close(rook_state_t *R) {
}

/*----------------------------------------------------------------------*/
int rook_do_file(rook_state_t *R, const char *file_name) {
    char *source;
    size_t source_size;
    lex_output_t lex_output;
    parse_output_t parse_output;
    compile_output_t compile_output;

    /* read source code from file */
    if (!read_file(file_name, &source, &source_size)) {
        R->error = 1;
        goto fail;
    }

    /* lex */
    {
        lex_input_t lex_input;
        lex_input.source = source;
        lex_input.allocator = &R->allocator;
        lex(&lex_input, &lex_output);
        if (lex_output.error) {
            R->error = lex_output.error;
            goto fail;
        }
    }

    /* parse */
    {
        parse_input_t parse_input;
        parse_input.lex_out = &lex_output;
        parse_input.allocator = &R->allocator;
        parse(&parse_input, &parse_output);
        if (parse_output.error) {
            R->error = parse_output.error;
            goto fail;
        }
    }

    /* compile */
    {
        compile_input_t compile_input;
        compile_input.allocator = &R->allocator;
        compile_input.parse_out = &parse_output;
        compile(&compile_input, &compile_output);
        if (compile_output.error) {
            R->error = compile_output.error;
            goto fail;
        }
    }

    /* dump bytecode */
    {
        size_t i;
        size_t num_bytes = compile_output.bytestream.ptr - compile_output.bytestream.start;
        uint8_t *ptr = compile_output.bytestream.start;

        for (i = 0; i < num_bytes; ++i) {
            if (( i % 16) == 0) {
                printf("\n%04ld: ", i);
            }
            printf("%02X ", ptr[i]);
        }
        printf("\n\n");
    }

    /* disassemble */
    {
        size_t size = compile_output.bytestream.ptr - compile_output.bytestream.start;
        disasm(compile_output.bytestream.start, size);
    }

    /* run */
    {
        vm_t vm;
        function_table_entry_t *entry;

        /* look up function "main" */
        entry = hash_table_find(compile_output.function_table, "main");
        if (entry == NULL) {
            printf("main not found\n");
            goto fail;
        }

        /* set up vm */
        vm.bytecode = compile_output.bytestream.start;
        vm.bytecode_size = compile_output.bytestream.ptr - compile_output.bytestream.start;
        vm.stack = ALLOCATOR_ALLOC(&R->allocator, 1024 * 16);
        vm.ip = entry->address;
        vm.sp = 0;
        vm.bp = vm.sp;
        stack_push_si32(&vm, 0);  /* return value */
        stack_push_ui32(&vm, ~0); /* return address */

        /* run vm */
        vm_run(&vm);

        /* get return value of main */
        printf("result: %d\n", stack_pop_si32(&vm));
    }

    R->error = 0;
    return R->error;

fail:
    return R->error;
}

/*----------------------------------------------------------------------*/
int rook_get_error(rook_state_t *R) {
    return R->error;
}

/*----------------------------------------------------------------------*/
const char *rook_get_error_str(rook_state_t *R) {
    return R->error_str;
}
