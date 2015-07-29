#pragma once
#include "bytestream.h"

typedef struct {
    uint8_t *bytecode;
    size_t bytecode_size;
    uint8_t *stack;
    size_t ip;
    size_t sp;
    size_t bp;
} vm_t;

void vm_run(vm_t *vm);
