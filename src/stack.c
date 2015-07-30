#include "stack.h"
#include <string.h>

/*----------------------------------------------------------------------*/
void stack_push_n(vm_t *vm, void *p, size_t n) {
    memcpy(vm->stack + vm->sp, p, n);
    vm->sp += n;
}

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
    *((int32_t *)(&vm->stack[vm->sp])) = value;
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
    *((uint32_t *)(&vm->stack[vm->sp])) = value;
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
void stack_pop_n(vm_t *vm, void *dest, size_t size) {
    vm->sp -= size;
    if (dest != NULL) {
        uint8_t *src = vm->stack + vm->sp;
        memcpy(dest, src, size);
    }
}

/*----------------------------------------------------------------------*/
int8_t stack_pop_si8(vm_t *vm) {
    vm->sp -= sizeof(int8_t);
    return *((int8_t *)&vm->stack[vm->sp]);
}

/*----------------------------------------------------------------------*/
int16_t stack_pop_si16(vm_t *vm) {
    vm->sp -= sizeof(int16_t);
    return *((int16_t *)&vm->stack[vm->sp]);
}

/*----------------------------------------------------------------------*/
int32_t stack_pop_si32(vm_t *vm) {
    vm->sp -= sizeof(int32_t);
    return *((int32_t *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
int64_t stack_pop_si64(vm_t *vm) {
    vm->sp -= sizeof(int64_t);
    return *((int64_t *)&vm->stack[vm->sp]);
}

/*----------------------------------------------------------------------*/
uint8_t stack_pop_ui8(vm_t *vm) {
    vm->sp -= sizeof(uint8_t);
    return*((uint8_t *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
uint16_t stack_pop_ui16(vm_t *vm) {
    vm->sp -= sizeof(uint16_t);
    return *((uint16_t *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
uint32_t stack_pop_ui32(vm_t *vm) {
    vm->sp -= sizeof(uint32_t);
    return *((uint32_t *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
uint64_t stack_pop_ui64(vm_t *vm) {
    vm->sp -= sizeof(uint64_t);
    return *((uint64_t *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
float stack_pop_f(vm_t *vm) {
    vm->sp -= sizeof(float);
    return *((float *)(&vm->stack[vm->sp]));
}

/*----------------------------------------------------------------------*/
double stack_pop_d(vm_t *vm) {
    vm->sp -= sizeof(double);
    return *((double *)(&vm->stack[vm->sp]));
}
