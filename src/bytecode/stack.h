#pragma once
#include "vm.h"
#include <stddef.h>

void stack_push_n(vm_t *vm, void *p, size_t n);
void stack_push_si8(vm_t *vm, int8_t value);
void stack_push_si16(vm_t *vm, int16_t value);
void stack_push_si32(vm_t *vm, int32_t value);
void stack_push_si64(vm_t *vm, int64_t value);
void stack_push_ui8(vm_t *vm, uint8_t value);
void stack_push_ui16(vm_t *vm, uint16_t value);
void stack_push_ui32(vm_t *vm, uint32_t value);
void stack_push_ui64(vm_t *vm, uint64_t value);
void stack_push_f(vm_t *vm, float value);
void stack_push_d(vm_t *vm, double value);
void stack_pop_n(vm_t *vm, void *dest, size_t size);
int8_t stack_pop_si8(vm_t *vm);
int16_t stack_pop_si16(vm_t *vm);
int32_t stack_pop_si32(vm_t *vm);
int64_t stack_pop_si64(vm_t *vm);
uint8_t stack_pop_ui8(vm_t *vm);
uint16_t stack_pop_ui16(vm_t *vm);
uint32_t stack_pop_ui32(vm_t *vm);
uint64_t stack_pop_ui64(vm_t *vm);
float stack_pop_f(vm_t *vm);
double stack_pop_d(vm_t *vm);
