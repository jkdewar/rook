#include "vm.h"
#include "bytecode.h"
#include <stdio.h>

/*----------------------------------------------------------------------*/
static void print_subtype(opcode_subtype_t subtype) {
    switch (subtype) {
        case OP_ST_SI32: printf("SI32"); break;
        default: printf("???"); break;
    }
}

/*----------------------------------------------------------------------*/
void vm_run(vm_t *vm) {
    opcode_t opcode;
    opcode_subtype_t subtype;

    printf("\nRunning VM...\n");

    vm->ip = 0;
    for (;;) {
        if (vm->ip >= vm->bytecode_stream->ptr - vm->bytecode_stream->start)
            break;
        opcode = vm->bytecode_stream->start[vm->ip];
        vm->ip += sizeof(uint8_t);
        switch (opcode) {
            case OP_PUSH:
                subtype = vm->bytecode_stream->start[vm->ip];
                vm->ip += sizeof(uint8_t);
                printf("PUSH.");
                print_subtype(subtype);
                vm->ip += 4; /* TODO:jkd */
                break;
            case OP_RET:
                printf("RET");
                break;
            case OP_ADD:
                printf("ADD.");
                subtype = vm->bytecode_stream->start[vm->ip];
                vm->ip += sizeof(uint8_t);
                print_subtype(subtype);
                break;
            case OP_SUB:
                printf("SUB.");
                subtype = vm->bytecode_stream->start[vm->ip];
                vm->ip += sizeof(uint8_t);
                print_subtype(subtype);
                break;
            case OP_MUL:
                printf("MUL.");
                subtype = vm->bytecode_stream->start[vm->ip];
                vm->ip += sizeof(uint8_t);
                print_subtype(subtype);
                break;
            case OP_DIV:
                printf("DIV.");
                subtype = vm->bytecode_stream->start[vm->ip];
                vm->ip += sizeof(uint8_t);
                print_subtype(subtype);
                break;
            default:
                printf("OP_??? ($%02X)", opcode);
                break;
        }
        printf("\n");
    }
}
