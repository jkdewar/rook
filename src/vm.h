#pragma once
#include "bytestream.h"

typedef struct {
    bytestream_t *bytecode_stream;
    size_t ip;
} vm_t;

void vm_run(vm_t *vm);
