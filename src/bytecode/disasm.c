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
void disasm(uint8_t *bytecode, size_t size) {
    instruction_t *inst;
    size_t ip = 0;
    while (ip < size) {
        inst = (instruction_t *) &bytecode[ip];
        ip += sizeof(instruction_t);
        switch (inst->opcode) {
            case OP_PUSH:
                printf("PUSH.");
                print_subtype(inst->subtype);
                printf(" ");
                switch (inst->subtype) {
                    case OP_ST_SI8 : printf("%d"   , inst->u.push_si8 .value);
                    case OP_ST_SI16: printf("%d"   , inst->u.push_si16.value);
                    case OP_ST_SI32: printf("%d"   , inst->u.push_si32.value);
                    case OP_ST_SI64: printf("%lld" , inst->u.push_si64.value);
                    case OP_ST_UI8 : printf("%ud"  , inst->u.push_ui8 .value);
                    case OP_ST_UI16: printf("%ud"  , inst->u.push_ui16.value);
                    case OP_ST_UI32: printf("%ud"  , inst->u.push_ui32.value);
                    case OP_ST_UI64: printf("%ulld", inst->u.push_ui64.value);
                    case OP_ST_F   : printf("%f"   , inst->u.push_f   .value);
                    case OP_ST_D   : printf("%f"   , inst->u.push_d   .value);
                }
                break;
            case OP_ADD:
                printf("PUSH.");
                print_subtype(inst->subtype);
                break;
            case OP_SUB:
                printf("SUB.");
                print_subtype(inst->subtype);
                break;
            case OP_MUL:
                printf("MUL.");
                print_subtype(inst->subtype);
                break;
            case OP_DIV:
                printf("DIV.");
                print_subtype(inst->subtype);
                break;
            case OP_RET:
                printf("RET");
                break;
            default:
                printf("OP_??? (%02X)", inst->opcode);
                break;
        }
        printf("\n");
    }
}