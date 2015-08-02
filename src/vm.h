#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bytecode;
    size_t bytecode_size;
    uint8_t *stack;
    uint32_t ip;
    uint32_t sp;
    uint32_t bp;
} vm_t;

void vm_run(vm_t *vm);
