#include "bytecode.h"
#include <stdio.h>
#include <string.h>

#define PUSH_OP(OP) bytestream_push8(bs, (uint8_t)(OP))
#define PUSH_ST(ST) bytestream_push8(bs, (uint8_t)(ST))
#define PUSH_8 (X) bytestream_push8 (bs, X)
#define PUSH_16(X) bytestream_push16(bs, X)
#define PUSH_32(X) bytestream_push32(bs, X)
#define PUSH_64(X) bytestream_push64(bs, X)

/*----------------------------------------------------------------------*/
void bcbuild_STORE(bytestream_t *bs, uint32_t size, int32_t stack_pos) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_STORE;
    i.u.store.size = size;
    i.u.store.stack_pos = stack_pos;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_FRAME(bytestream_t *bs, uint32_t size, uint32_t *size_loc) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_FRAME;
    i.u.frame.size = size;
    if (size_loc != NULL)
        *size_loc = bytestream_loc(bs);
    bytestream_pushn(bs, &i, sizeof(instruction_t));
    if (size_loc != NULL)
        *size_loc += (uint32_t)((uint8_t*)&i.u.frame.size - (uint8_t*)&i);
}

/*----------------------------------------------------------------------*/
void bcbuild_RET(bytestream_t *bs) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_RET;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_J(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    PUSH_OP(OP_J);
    if (address_loc != NULL)
        *address_loc = bytestream_loc(bs);
    PUSH_32(address);
}

/*----------------------------------------------------------------------*/
void bcbuild_JT(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    PUSH_OP(OP_JT);
    if (address_loc != NULL)
        *address_loc = bytestream_loc(bs);
    PUSH_32(address);
}

/*----------------------------------------------------------------------*/
void bcbuild_JF(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    PUSH_OP(OP_JF);
    if (address_loc != NULL)
        *address_loc = bytestream_loc(bs);
    PUSH_32(address);
}

/*----------------------------------------------------------------------*/
void bcbuild_PUSH_SI32(bytestream_t *bs, int32_t value) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_PUSH;
    i.subtype = OP_ST_SI32;
    i.u.push_si32.value = value;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_ADD;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_SUB;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_MUL;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st) {
    instruction_t i;
    memset(&i, 0xff, sizeof(instruction_t));
    i.opcode = OP_DIV;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}
