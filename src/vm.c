#include "vm.h"
#include "bytecode.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

#define PUSH_BOOL(X) stack_push_ui8 (vm, X)
#define PUSH_SI8(X)  stack_push_si8 (vm, X)
#define PUSH_SI16(X) stack_push_si16(vm, X)
#define PUSH_SI32(X) stack_push_si32(vm, X)
#define PUSH_SI64(X) stack_push_si64(vm, X)
#define PUSH_UI8(X)  stack_push_ui8 (vm, X)
#define PUSH_UI16(X) stack_push_ui16(vm, X)
#define PUSH_UI32(X) stack_push_ui32(vm, X)
#define PUSH_UI64(X) stack_push_ui64(vm, X)
#define PUSH_F(X)    stack_push_f   (vm, X)
#define PUSH_D(X)    stack_push_d   (vm, X)

#define POP_BOOL() stack_pop_ui8 (vm)
#define POP_SI8()  stack_pop_si8 (vm)
#define POP_SI16() stack_pop_si16(vm)
#define POP_SI32() stack_pop_si32(vm)
#define POP_SI64() stack_pop_si64(vm)
#define POP_UI8()  stack_pop_ui8 (vm)
#define POP_UI16() stack_pop_ui16(vm)
#define POP_UI32() stack_pop_ui32(vm)
#define POP_UI64() stack_pop_ui64(vm)
#define POP_F()    stack_pop_f   (vm)
#define POP_D()    stack_pop_d   (vm)

/*----------------------------------------------------------------------*/
void vm_run(vm_t *vm) {
    instruction_t *inst;

    printf("\nRunning VM...\n");

    vm->ip = 0;

    for (;;) {
        if (vm->ip >= vm->bytecode_size)
            break;
        inst = (instruction_t *) &vm->bytecode[vm->ip];
        vm->ip += sizeof(instruction_t);
#define P(OP, ST) ((OP) + ((ST) << 8))
        switch P(inst->opcode, inst->subtype) {
/*- STORE --------------------------------------------------------------*/
            case P(OP_STORE, 0xFF): {
                uint8_t *dest = vm->stack + vm->bp + inst->u.store.stack_pos;
                stack_pop_n(vm, dest, inst->u.store.size);
                break;
            }
/*- LOAD ---------------------------------------------------------------*/
            case P(OP_LOAD, 0xFF): {
                uint8_t *src = vm->stack + vm->bp + inst->u.store.stack_pos;
                stack_push_n(vm, src, inst->u.store.size);
                break;
            }
/*- ADD ----------------------------------------------------------------*/
            case P(OP_ADD, OP_ST_SI8) : PUSH_SI8 (POP_SI8 () + POP_SI8 ()); break;
            case P(OP_ADD, OP_ST_SI16): PUSH_SI16(POP_SI16() + POP_SI16()); break;
            case P(OP_ADD, OP_ST_SI32): PUSH_SI32(POP_SI32() + POP_SI32()); break;
            case P(OP_ADD, OP_ST_SI64): PUSH_SI64(POP_SI64() + POP_SI64()); break;
            case P(OP_ADD, OP_ST_UI8 ): PUSH_UI8 (POP_UI8 () + POP_UI8 ()); break;
            case P(OP_ADD, OP_ST_UI16): PUSH_UI16(POP_UI16() + POP_UI16()); break;
            case P(OP_ADD, OP_ST_UI32): PUSH_UI32(POP_UI32() + POP_UI32()); break;
            case P(OP_ADD, OP_ST_UI64): PUSH_UI64(POP_UI64() + POP_UI64()); break;
            case P(OP_ADD, OP_ST_F   ): PUSH_F   (POP_F   () + POP_F   ()); break;
            case P(OP_ADD, OP_ST_D   ): PUSH_D   (POP_D   () + POP_D   ()); break;
/*- SUB ----------------------------------------------------------------*/
            case P(OP_SUB, OP_ST_SI8 ): PUSH_SI8 (POP_SI8 () - POP_SI8 ()); break;
            case P(OP_SUB, OP_ST_SI16): PUSH_SI16(POP_SI16() - POP_SI16()); break;
            case P(OP_SUB, OP_ST_SI32): PUSH_SI32(POP_SI32() - POP_SI32()); break;
            case P(OP_SUB, OP_ST_SI64): PUSH_SI64(POP_SI64() - POP_SI64()); break;
            case P(OP_SUB, OP_ST_UI8 ): PUSH_UI8 (POP_UI8 () - POP_UI8 ()); break;
            case P(OP_SUB, OP_ST_UI16): PUSH_UI16(POP_UI16() - POP_UI16()); break;
            case P(OP_SUB, OP_ST_UI32): PUSH_UI32(POP_UI32() - POP_UI32()); break;
            case P(OP_SUB, OP_ST_UI64): PUSH_UI64(POP_UI64() - POP_UI64()); break;
            case P(OP_SUB, OP_ST_F   ): PUSH_F   (POP_F   () - POP_F   ()); break;
            case P(OP_SUB, OP_ST_D   ): PUSH_D   (POP_D   () - POP_D   ()); break;
/*- MUL ----------------------------------------------------------------*/
            case P(OP_MUL, OP_ST_SI8 ): PUSH_SI8 (POP_SI8 () * POP_SI8 ()); break;
            case P(OP_MUL, OP_ST_SI16): PUSH_SI16(POP_SI16() * POP_SI16()); break;
            case P(OP_MUL, OP_ST_SI32): PUSH_SI32(POP_SI32() * POP_SI32()); break;
            case P(OP_MUL, OP_ST_SI64): PUSH_SI64(POP_SI64() * POP_SI64()); break;
            case P(OP_MUL, OP_ST_UI8 ): PUSH_UI8 (POP_UI8 () * POP_UI8 ()); break;
            case P(OP_MUL, OP_ST_UI16): PUSH_UI16(POP_UI16() * POP_UI16()); break;
            case P(OP_MUL, OP_ST_UI32): PUSH_UI32(POP_UI32() * POP_UI32()); break;
            case P(OP_MUL, OP_ST_UI64): PUSH_UI64(POP_UI64() * POP_UI64()); break;
            case P(OP_MUL, OP_ST_F   ): PUSH_F   (POP_F   () * POP_F   ()); break;
            case P(OP_MUL, OP_ST_D   ): PUSH_D   (POP_D   () * POP_D   ()); break;
/*- DIV ----------------------------------------------------------------*/
            case P(OP_DIV, OP_ST_SI8 ): PUSH_SI8 (POP_SI8 () / POP_SI8 ()); break;
            case P(OP_DIV, OP_ST_SI16): PUSH_SI16(POP_SI16() / POP_SI16()); break;
            case P(OP_DIV, OP_ST_SI32): PUSH_SI32(POP_SI32() / POP_SI32()); break;
            case P(OP_DIV, OP_ST_SI64): PUSH_SI64(POP_SI64() / POP_SI64()); break;
            case P(OP_DIV, OP_ST_UI8 ): PUSH_UI8 (POP_UI8 () / POP_UI8 ()); break;
            case P(OP_DIV, OP_ST_UI16): PUSH_UI16(POP_UI16() / POP_UI16()); break;
            case P(OP_DIV, OP_ST_UI32): PUSH_UI32(POP_UI32() / POP_UI32()); break;
            case P(OP_DIV, OP_ST_UI64): PUSH_UI64(POP_UI64() / POP_UI64()); break;
            case P(OP_DIV, OP_ST_F   ): PUSH_F   (POP_F   () / POP_F   ()); break;
            case P(OP_DIV, OP_ST_D   ): PUSH_D   (POP_D   () / POP_D   ()); break;
/*- PUSH ---------------------------------------------------------------*/
            case P(OP_PUSH, OP_ST_SI8 ): PUSH_SI8 (inst->u.push_si8 .value); break;
            case P(OP_PUSH, OP_ST_SI16): PUSH_SI16(inst->u.push_si16.value); break;
            case P(OP_PUSH, OP_ST_SI32): PUSH_SI32(inst->u.push_si32.value); break;
            case P(OP_PUSH, OP_ST_SI64): PUSH_SI64(inst->u.push_si64.value); break;
            case P(OP_PUSH, OP_ST_UI8 ): PUSH_UI8 (inst->u.push_ui8 .value); break;
            case P(OP_PUSH, OP_ST_UI16): PUSH_UI16(inst->u.push_ui16.value); break;
            case P(OP_PUSH, OP_ST_UI32): PUSH_UI32(inst->u.push_ui32.value); break;
            case P(OP_PUSH, OP_ST_UI64): PUSH_UI64(inst->u.push_ui64.value); break;
            case P(OP_PUSH, OP_ST_F   ): PUSH_F   (inst->u.push_f   .value); break;
            case P(OP_PUSH, OP_ST_D   ): PUSH_D   (inst->u.push_d   .value); break;
/*- PUSHZ --------------------------------------------------------------*/
            case P(OP_PUSHZ, 0xFF): {
                memset(vm->stack + vm->sp, 0, inst->u.pushz.count);
                vm->sp += inst->u.pushz.count;
                break;
            }
/*- CALL ---------------------------------------------------------------*/
            case P(OP_CALL, 0xFF): {
                PUSH_UI32(vm->ip);
                vm->ip = inst->u.call.address;
                break;
            }
/*- FRAME --------------------------------------------------------------*/
            case P(OP_FRAME, 0xFF): {
                PUSH_UI32(vm->sp);
                PUSH_UI32(vm->bp);
                vm->bp = vm->sp;
                vm->sp += inst->u.frame.size;
                break;
            }
/*- RET ----------------------------------------------------------------*/
            case P(OP_RET, 0xFF): {
                /*  TODO:jkd move return value above the frame */
                vm->sp = vm->bp;
                vm->bp = POP_UI32();
                vm->sp = POP_UI32();
                vm->ip = POP_UI32();
                break;
            }
/*- J ------------------------------------------------------------------*/
            case P(OP_J, 0xFF): {
                vm->ip = inst->u.j.address;
                break;
            }
/*- JT -----------------------------------------------------------------*/
            case P(OP_JT, 0xFF): {
                if (POP_UI8() != 0) {
                    vm->ip = inst->u.j.address;
                }
                break;
            }
/*- JF -----------------------------------------------------------------*/
            case P(OP_JF, 0xFF): {
                if (POP_UI8() == 0) {
                    vm->ip = inst->u.j.address;
                }
                break;
            }
/*- TLE ----------------------------------------------------------------*/
            case P(OP_TL, OP_ST_SI8 ): PUSH_BOOL(POP_SI8 () < POP_SI8 ()); break;
            case P(OP_TL, OP_ST_SI16): PUSH_BOOL(POP_SI16() < POP_SI16()); break;
            case P(OP_TL, OP_ST_SI32): PUSH_BOOL(POP_SI32() < POP_SI32()); break;
            case P(OP_TL, OP_ST_SI64): PUSH_BOOL(POP_SI64() < POP_SI64()); break;
            case P(OP_TL, OP_ST_UI8 ): PUSH_BOOL(POP_UI8 () < POP_UI8 ()); break;
            case P(OP_TL, OP_ST_UI16): PUSH_BOOL(POP_UI16() < POP_UI16()); break;
            case P(OP_TL, OP_ST_UI32): PUSH_BOOL(POP_UI32() < POP_UI32()); break;
            case P(OP_TL, OP_ST_UI64): PUSH_BOOL(POP_UI64() < POP_UI64()); break;
            case P(OP_TL, OP_ST_F   ): PUSH_BOOL(POP_F   () < POP_F   ()); break;
            case P(OP_TL, OP_ST_D   ): PUSH_BOOL(POP_D   () < POP_D   ()); break;
/*- TLE ----------------------------------------------------------------*/
            case P(OP_TLE, OP_ST_SI8 ): PUSH_BOOL(POP_SI8 () <= POP_SI8 ()); break;
            case P(OP_TLE, OP_ST_SI16): PUSH_BOOL(POP_SI16() <= POP_SI16()); break;
            case P(OP_TLE, OP_ST_SI32): PUSH_BOOL(POP_SI32() <= POP_SI32()); break;
            case P(OP_TLE, OP_ST_SI64): PUSH_BOOL(POP_SI64() <= POP_SI64()); break;
            case P(OP_TLE, OP_ST_UI8 ): PUSH_BOOL(POP_UI8 () <= POP_UI8 ()); break;
            case P(OP_TLE, OP_ST_UI16): PUSH_BOOL(POP_UI16() <= POP_UI16()); break;
            case P(OP_TLE, OP_ST_UI32): PUSH_BOOL(POP_UI32() <= POP_UI32()); break;
            case P(OP_TLE, OP_ST_UI64): PUSH_BOOL(POP_UI64() <= POP_UI64()); break;
            case P(OP_TLE, OP_ST_F   ): PUSH_BOOL(POP_F   () <= POP_F   ()); break;
            case P(OP_TLE, OP_ST_D   ): PUSH_BOOL(POP_D   () <= POP_D   ()); break;
/*- TE -----------------------------------------------------------------*/
            case P(OP_TE, OP_ST_SI8 ): PUSH_BOOL(POP_SI8 () == POP_SI8 ()); break;
            case P(OP_TE, OP_ST_SI16): PUSH_BOOL(POP_SI16() == POP_SI16()); break;
            case P(OP_TE, OP_ST_SI32): PUSH_BOOL(POP_SI32() == POP_SI32()); break;
            case P(OP_TE, OP_ST_SI64): PUSH_BOOL(POP_SI64() == POP_SI64()); break;
            case P(OP_TE, OP_ST_UI8 ): PUSH_BOOL(POP_UI8 () == POP_UI8 ()); break;
            case P(OP_TE, OP_ST_UI16): PUSH_BOOL(POP_UI16() == POP_UI16()); break;
            case P(OP_TE, OP_ST_UI32): PUSH_BOOL(POP_UI32() == POP_UI32()); break;
            case P(OP_TE, OP_ST_UI64): PUSH_BOOL(POP_UI64() == POP_UI64()); break;
            case P(OP_TE, OP_ST_F   ): PUSH_BOOL(POP_F   () == POP_F   ()); break;
            case P(OP_TE, OP_ST_D   ): PUSH_BOOL(POP_D   () == POP_D   ()); break;
/*- TGE ----------------------------------------------------------------*/
            case P(OP_TGE, OP_ST_SI8 ): PUSH_BOOL(POP_SI8 () >= POP_SI8 ()); break;
            case P(OP_TGE, OP_ST_SI16): PUSH_BOOL(POP_SI16() >= POP_SI16()); break;
            case P(OP_TGE, OP_ST_SI32): PUSH_BOOL(POP_SI32() >= POP_SI32()); break;
            case P(OP_TGE, OP_ST_SI64): PUSH_BOOL(POP_SI64() >= POP_SI64()); break;
            case P(OP_TGE, OP_ST_UI8 ): PUSH_BOOL(POP_UI8 () >= POP_UI8 ()); break;
            case P(OP_TGE, OP_ST_UI16): PUSH_BOOL(POP_UI16() >= POP_UI16()); break;
            case P(OP_TGE, OP_ST_UI32): PUSH_BOOL(POP_UI32() >= POP_UI32()); break;
            case P(OP_TGE, OP_ST_UI64): PUSH_BOOL(POP_UI64() >= POP_UI64()); break;
            case P(OP_TGE, OP_ST_F   ): PUSH_BOOL(POP_F   () >= POP_F   ()); break;
            case P(OP_TGE, OP_ST_D   ): PUSH_BOOL(POP_D   () >= POP_D   ()); break;
/*- TG -----------------------------------------------------------------*/
            case P(OP_TG, OP_ST_SI8 ): PUSH_BOOL(POP_SI8 () > POP_SI8 ()); break;
            case P(OP_TG, OP_ST_SI16): PUSH_BOOL(POP_SI16() > POP_SI16()); break;
            case P(OP_TG, OP_ST_SI32): PUSH_BOOL(POP_SI32() > POP_SI32()); break;
            case P(OP_TG, OP_ST_SI64): PUSH_BOOL(POP_SI64() > POP_SI64()); break;
            case P(OP_TG, OP_ST_UI8 ): PUSH_BOOL(POP_UI8 () > POP_UI8 ()); break;
            case P(OP_TG, OP_ST_UI16): PUSH_BOOL(POP_UI16() > POP_UI16()); break;
            case P(OP_TG, OP_ST_UI32): PUSH_BOOL(POP_UI32() > POP_UI32()); break;
            case P(OP_TG, OP_ST_UI64): PUSH_BOOL(POP_UI64() > POP_UI64()); break;
            case P(OP_TG, OP_ST_F   ): PUSH_BOOL(POP_F   () > POP_F   ()); break;
            case P(OP_TG, OP_ST_D   ): PUSH_BOOL(POP_D   () > POP_D   ()); break;
        }
    }
}
