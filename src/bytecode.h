#pragma once
#include "bytestream.h"
#include <stdint.h>

typedef enum {
    OP_PUSHZ        = 0x00,
    OP_STORE        = 0x01,
    OP_LOAD         = 0x02,
    OP_CALL         = 0x03,
    OP_FRAME        = 0x04,
    OP_RET          = 0x05,
    OP_J            = 0x06,
    OP_JT           = 0x07,
    OP_JF           = 0x08,
    OP_LGCAND       = 0x09,
    OP_LGCOR        = 0x0A,
/* with subtype */
    OP_PUSH         = 0x0B,
    OP_POP          = 0x0C,
    OP_ADD          = 0x0D,
    OP_SUB          = 0x0E,
    OP_MUL          = 0x0F,
    OP_DIV          = 0x10,
    OP_TE           = 0x11,
    OP_TL           = 0x12,
    OP_TLE          = 0x13,
    OP_TG           = 0x14,
    OP_TGE          = 0x15
} opcode_t;

typedef enum {
    OP_ST_SI8       = 0x00,
    OP_ST_SI16      = 0x01,
    OP_ST_SI32      = 0x02,
    OP_ST_SI64      = 0x03,
    OP_ST_UI8       = 0x04,
    OP_ST_UI16      = 0x05,
    OP_ST_UI32      = 0x06,
    OP_ST_UI64      = 0x07,
    OP_ST_F         = 0x08,
    OP_ST_D         = 0x09
} opcode_subtype_t;

typedef union {
    struct { uint32_t count; } pushz;
    struct { uint32_t size; int32_t stack_pos; } store, load;
    struct { uint32_t address; } call;
    struct { uint32_t size; } frame;
    struct { uint32_t address; } j, jt, jf;
    struct { int8_t value; } push_si8;
    struct { int16_t value; } push_si16;
    struct { int32_t value; } push_si32;
    struct { int64_t value; } push_si64;
    struct { uint8_t value; } push_ui8;
    struct { uint16_t value; } push_ui16;
    struct { uint32_t value; } push_ui32;
    struct { uint32_t value; } push_ui64;
    struct { float value; } push_f;
    struct { double value; } push_d;
} instruction_union_t;

typedef struct {
    uint8_t opcode;
    uint8_t subtype;
    instruction_union_t u;
} instruction_t;

void bcbuild_STORE(bytestream_t *bs, uint32_t size, int32_t stack_pos);
void bcbuild_FRAME(bytestream_t *bs, uint32_t size, uint32_t *size_where);
void bcbuild_RET(bytestream_t *bs);
void bcbuild_J(bytestream_t *bs, uint32_t address, uint32_t *address_where);
void bcbuild_JT(bytestream_t *bs, uint32_t address, uint32_t *address_where);
void bcbuild_JF(bytestream_t *bs, uint32_t address, uint32_t *address_where);
void bcbuild_PUSH_SI32(bytestream_t *bs, int32_t value);
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_TL(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_TLE(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_TE(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_TGE(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_TG(bytestream_t *bs, opcode_subtype_t st);
