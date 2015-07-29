#include "vm.h"
#include "bytecode.h"
#include <stdio.h>

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

#define POP_SI8()  stack_pop_si8 (vm)
#define POP_SI16() stack_pop_si16(vm)
#define POP_SI32() stack_pop_si32(vm)
#define POP_SI64() stack_pop_si64(vm)
#define POP_UI8( ) stack_pop_ui8 (vm)
#define POP_UI16() stack_pop_ui16(vm)
#define POP_UI32() stack_pop_ui32(vm)
#define POP_UI64() stack_pop_ui64(vm)
#define POP_F()    stack_pop_f   (vm)
#define POP_D()    stack_pop_d   (vm)

/*----------------------------------------------------------------------*/
void stack_push_si8(vm_t *vm, int8_t value) {
    *((int8_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_si16(vm_t *vm, int16_t value) {
    *((int16_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_si32(vm_t *vm, int32_t value) {
    *((int32_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_si64(vm_t *vm, int64_t value) {
    *((int64_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_ui8(vm_t *vm, uint8_t value) {
    *((uint8_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_ui16(vm_t *vm, uint16_t value) {
    *((uint16_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_ui32(vm_t *vm, uint32_t value) {
    *((uint32_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_ui64(vm_t *vm, uint64_t value) {
    *((uint64_t *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_f(vm_t *vm, float value) {
    *((float *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
void stack_push_d(vm_t *vm, double value) {
    *((double *)&vm->stack[vm->sp]) = value;
    vm->sp += sizeof(value);
}

/*----------------------------------------------------------------------*/
int8_t stack_pop_si8(vm_t *vm) {
    int8_t v = *((int8_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
int16_t stack_pop_si16(vm_t *vm) {
    int16_t v = *((int16_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
int32_t stack_pop_si32(vm_t *vm) {
    int32_t v = *((int32_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
int64_t stack_pop_si64(vm_t *vm) {
    int64_t v = *((int64_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
uint8_t stack_pop_ui8(vm_t *vm) {
    uint8_t v = *((uint8_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
uint16_t stack_pop_ui16(vm_t *vm) {
    uint16_t v = *((uint16_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
uint32_t stack_pop_ui32(vm_t *vm) {
    uint32_t v = *((uint32_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
uint64_t stack_pop_ui64(vm_t *vm) {
    uint64_t v = *((uint64_t *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
float stack_pop_f(vm_t *vm) {
    float v = *((float *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
double stack_pop_d(vm_t *vm) {
    double v = *((double *)&vm->stack[vm->sp]);
    vm->sp += sizeof(v);
    return v;
}

/*----------------------------------------------------------------------*/
void vm_run(vm_t *vm) {
    instruction_t *inst;

    printf("\nRunning VM...\n");

    vm->ip = 0;
    for (;;) {
        inst = (instruction_t *) &vm->bytecode[vm->ip];
        vm->ip += sizeof(instruction_t);
#define P(OP, ST) ((OP) + ((ST) << 8))
        switch P(inst->opcode, inst->subtype) {
/* ADD */
            case P(OP_ADD, OP_ST_SI8) : PUSH_SI8 (POP_SI8() + POP_SI8()); break;
            case P(OP_ADD, OP_ST_SI16): PUSH_SI16(POP_SI16() + POP_SI16()); break;
            case P(OP_ADD, OP_ST_SI32): PUSH_SI32(POP_SI32() + POP_SI32()); break;
            case P(OP_ADD, OP_ST_SI64): PUSH_SI64(POP_SI64() + POP_SI64()); break;
            case P(OP_ADD, OP_ST_UI8 ): PUSH_UI8 (POP_UI8 () + POP_UI8 ()); break;
            case P(OP_ADD, OP_ST_UI16): PUSH_UI16(POP_UI16() + POP_UI16()); break;
            case P(OP_ADD, OP_ST_UI32): PUSH_UI32(POP_UI32() + POP_UI32()); break;
            case P(OP_ADD, OP_ST_UI64): PUSH_UI64(POP_UI64() + POP_UI64()); break;
            case P(OP_ADD, OP_ST_F   ): PUSH_F   (POP_F   () + POP_F   ()); break;
            case P(OP_ADD, OP_ST_D   ): PUSH_D   (POP_D   () + POP_D   ()); break;
/* SUB */
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
/* MUL */
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
/* DIV */
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


        }
    }
}
