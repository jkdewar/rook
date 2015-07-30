#include "bass.h"
#include "filesys.h"
#include "lex.h"
#include "parse.h"
#include "compile.h"
#include "disasm.h"
#include "vm.h"
#include "stack.h"
#include <stdio.h>

struct bass_state_t {
    allocator_t allocator;
    int error;
    char error_str[1024];
};

/*----------------------------------------------------------------------*/
bass_state_t *bass_open(allocator_t allocator) {
    bass_state_t *B = ALLOCATOR_ALLOC(&allocator, sizeof(bass_state_t));
    B->allocator = allocator;
    B->error = 0;
    B->error_str[0] = '\0';
    return B;
}

/*----------------------------------------------------------------------*/
void bass_close(bass_state_t *B) {
}

/*----------------------------------------------------------------------*/
int bass_do_file(bass_state_t *B, const char *file_name) {
    char *source;
    lex_output_t lex_output;
    parse_output_t parse_output;
    compile_output_t compile_output;

    /* read source code from file */
    if (!read_text_file(file_name, &source)) {
        B->error = 1;
        goto fail;
    }

    /* lex */
    {
        lex_input_t lex_input;
        lex_input.s = source;
        lex_input.allocator = &B->allocator;
        lex(&lex_input, &lex_output);
        if (lex_output.error) {
            B->error = lex_output.error;
            goto fail;
        }
    }

    /* parse */
    {
        parse_input_t parse_input;
        parse_input.lex_out = &lex_output;
        parse_input.allocator = &B->allocator;
        parse(&parse_input, &parse_output);
        if (parse_output.error) {
            B->error = parse_output.error;
            goto fail;
        }
    }

    /* compile */
    {
        compile_input_t compile_input;
        compile_input.allocator = &B->allocator;
        compile_input.parse_out = &parse_output;
        compile(&compile_input, &compile_output);
        if (compile_output.error) {
            B->error = compile_output.error;
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
                printf("\n%04lx: ", i);
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
        vm.stack = ALLOCATOR_ALLOC(&B->allocator, 1024 * 16);
        vm.ip = 0;
        vm.sp = 0;
        stack_push_ui32(&vm, 0);  /* return value */
        stack_push_ui32(&vm, ~0); /* return address */
        vm.bp = vm.sp;
        vm_run(&vm);
    }

    B->error = 0;
    return B->error;

fail:
    return B->error;
}

/*----------------------------------------------------------------------*/
int bass_get_error(bass_state_t *B) {
    return B->error;
}

/*----------------------------------------------------------------------*/
const char *bass_get_error_str(bass_state_t *B) {
    return B->error_str;
}
