#include "bytestream.h"
#include <string.h>

/*----------------------------------------------------------------------*/
void bytestream_init(bytestream_t *bs, size_t initial_capacity) {
    bs->start = ALLOCATOR_ALLOC(bs->allocator, initial_capacity);
    bs->ptr = bs->start;
    bs->capacity = initial_capacity;
}

/*----------------------------------------------------------------------*/
uint32_t bytestream_loc(bytestream_t *bs) {
    return (uint32_t)(bs->ptr - bs->start);
}

/*----------------------------------------------------------------------*/
void bytestream_pushn(bytestream_t *bs, void *data, size_t size) {
    /* TODO:jkd check if realloc is necessary */
    memcpy(bs->ptr, data, size);
    bs->ptr += size;
}

/*----------------------------------------------------------------------*/
void bytestream_push8(bytestream_t *bs, uint8_t data) {
    bytestream_pushn(bs, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_push16(bytestream_t *bs, uint16_t data) {
    bytestream_pushn(bs, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_push32(bytestream_t *bs, uint32_t data) {
    bytestream_pushn(bs, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_push64(bytestream_t *bs, uint64_t data) {
    bytestream_pushn(bs, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_setn(bytestream_t *bs, uint32_t loc, void *data, size_t size) {
    /* TODO:jkd check bounds */
    uint8_t *dest = bs->start + loc;
    memcpy(dest, data, size);
}

/*----------------------------------------------------------------------*/
void bytestream_set8(bytestream_t *bs, uint32_t loc, uint8_t data) {
    bytestream_setn(bs, loc, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_set16(bytestream_t *bs, uint32_t loc, uint16_t data) {
    bytestream_setn(bs, loc, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_set32(bytestream_t *bs, uint32_t loc, uint32_t data) {
    bytestream_setn(bs, loc, &data, sizeof(data));
}

/*----------------------------------------------------------------------*/
void bytestream_set64(bytestream_t *bs, uint32_t loc, uint64_t data) {
    bytestream_setn(bs, loc, &data, sizeof(data));
}
