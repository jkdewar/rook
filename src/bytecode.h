#pragma once
#include "bytestream.h"
#include <stdint.h>

typedef enum {
    OP_PUSHZ,
    OP_STORE,
    OP_LOAD,

    OP_CALL,
    OP_FRAME,
    OP_RET,

    OP_J,
    OP_JT,
    OP_JF,

    OP_LGCAND,
    OP_LGCOR,

/* with subtype */
    OP_PUSH,
    OP_POP,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_TE,
    OP_TL,
    OP_TLE,
    OP_TG,
    OP_TGE

} opcode_t;

typedef enum {
    OP_ST_SI8, OP_ST_SI16, OP_ST_SI32, OP_ST_SI64, /* signed int */
    OP_ST_UI8, OP_ST_UI16, OP_ST_UI32, OP_ST_UI64, /* unsigned int */
    OP_ST_F, /* float (32-bit) */
    OP_ST_D  /* double (64-bit) */
} opcode_subtype_t;

void bcbuild_RET(bytestream_t *bs);
void bcbuild_J(bytestream_t *bs, uint32_t address, uint32_t *address_loc);
void bcbuild_JT(bytestream_t *bs, uint32_t address, uint32_t *address_loc);
void bcbuild_JF(bytestream_t *bs, uint32_t address, uint32_t *address_loc);
void bcbuild_PUSH_SI32(bytestream_t *bs, int32_t value);
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st);
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st);
