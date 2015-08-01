#include "vm.h"
#include "bytecode.h"
#include <stdio.h>

/*----------------------------------------------------------------------*/
static void print_subtype(opcode_subtype_t subtype) {
    switch (subtype) {
        case OP_ST_SI8:  printf("SI8");  break;
        case OP_ST_SI16: printf("SI16"); break;
        case OP_ST_SI32: printf("SI32"); break;
        case OP_ST_SI64: printf("SI64"); break;
        case OP_ST_UI8:  printf("UI8");  break;
        case OP_ST_UI16: printf("UI16"); break;
        case OP_ST_UI32: printf("UI32"); break;
        case OP_ST_UI64: printf("UI64"); break;
        case OP_ST_F:    printf("F");    break;
        case OP_ST_D:    printf("D");    break;
        default: printf("???"); break;
    }
}

/*----------------------------------------------------------------------*/
void disasm(uint8_t *bytecode, size_t size) {
    instruction_t *inst;
    size_t ip = 0;
    while (ip < size) {
        printf("%04ld: ", ip);
        inst = (instruction_t *) &bytecode[ip];
        ip += sizeof(instruction_t);
        switch (inst->opcode) {
            case OP_PUSH:
                printf("PUSH.");
                print_subtype(inst->subtype);
                printf(" ");
                switch (inst->subtype) {
                    case OP_ST_SI8 : printf("%d"   , inst->u.push_si8 .value); break;
                    case OP_ST_SI16: printf("%d"   , inst->u.push_si16.value); break;
                    case OP_ST_SI32: printf("%d"   , inst->u.push_si32.value); break;
                    case OP_ST_SI64: printf("%lld" , inst->u.push_si64.value); break;
                    case OP_ST_UI8 : printf("%ud"  , inst->u.push_ui8 .value); break;
                    case OP_ST_UI16: printf("%ud"  , inst->u.push_ui16.value); break;
                    case OP_ST_UI32: printf("%ud"  , inst->u.push_ui32.value); break;
                    case OP_ST_UI64: printf("%ulld", inst->u.push_ui64.value); break;
                    case OP_ST_F   : printf("%f"   , inst->u.push_f   .value); break;
                    case OP_ST_D   : printf("%f"   , inst->u.push_d   .value); break;
                }
                break;
            case OP_ADD: printf("ADD."); print_subtype(inst->subtype); break;
            case OP_SUB: printf("SUB."); print_subtype(inst->subtype); break;
            case OP_MUL: printf("MUL."); print_subtype(inst->subtype); break;
            case OP_DIV: printf("DIV."); print_subtype(inst->subtype); break;
            case OP_TL:  printf("TL."); print_subtype(inst->subtype); break;
            case OP_TLE: printf("TLE."); print_subtype(inst->subtype); break;
            case OP_TE:  printf("TE."); print_subtype(inst->subtype); break;
            case OP_TGE: printf("TGE."); print_subtype(inst->subtype); break;
            case OP_TG:  printf("TG."); print_subtype(inst->subtype); break;
            case OP_RET: printf("RET"); break;
            case OP_FRAME: printf("FRAME %d", inst->u.frame.size); break;
            case OP_STORE: printf("STORE %d %d", inst->u.store.size, inst->u.store.stack_pos); break;
            case OP_LOAD:  printf("LOAD %d %d", inst->u.load.size, inst->u.load.stack_pos); break;
            case OP_J:  printf("J %d", inst->u.j.address); break;
            case OP_JF: printf("JF %d", inst->u.jf.address); break;
            case OP_JT: printf("JT %d", inst->u.jt.address); break;

            default:
                printf("OP_??? (0x%02X)", inst->opcode);
                break;
        }
        printf("\n");
    }
}
