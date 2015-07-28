#pragma once
#include "allocator/allocator.h"
#include <stdint.h>

typedef struct {
    uint8_t *start;
    size_t capacity;
    uint8_t *ptr;
    allocator_t allocator;
} bytestream_t;

uint32_t bytestream_loc(bytestream_t *bs);
void bytestream_pushn(bytestream_t *bs, void *data, size_t size);
void bytestream_push8(bytestream_t *bs, uint8_t data);
void bytestream_push16(bytestream_t *bs, uint16_t data);
void bytestream_push32(bytestream_t *bs, uint32_t data);
void bytestream_push64(bytestream_t *bs, uint64_t data);
void bytestream_setn(bytestream_t *bs, uint32_t loc, void *data, size_t size);
void bytestream_set8(bytestream_t *bs, uint32_t loc, uint8_t data);
void bytestream_set16(bytestream_t *bs, uint32_t loc, uint16_t data);
void bytestream_set32(bytestream_t *bs, uint32_t loc, uint32_t data);
void bytestream_set64(bytestream_t *bs, uint32_t loc, uint64_t data);
