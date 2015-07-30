#include "bytecode.h"
#include <stdio.h>
#include <string.h>

#define INST instruction_t i;  memset(&i, 0xff, sizeof(instruction_t));
#define PUSH_OP(OP) bytestream_push8(bs, (uint8_t)(OP))
#define PUSH_ST(ST) bytestream_push8(bs, (uint8_t)(ST))
#define PUSH_8 (X) bytestream_push8 (bs, X)
#define PUSH_16(X) bytestream_push16(bs, X)
#define PUSH_32(X) bytestream_push32(bs, X)
#define PUSH_64(X) bytestream_push64(bs, X)

/*----------------------------------------------------------------------*/
void bcbuild_STORE(bytestream_t *bs, uint32_t size, int32_t stack_pos) {
    INST
    i.opcode = OP_STORE;
    i.u.store.size = size;
    i.u.store.stack_pos = stack_pos;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_FRAME(bytestream_t *bs, uint32_t size, uint32_t *size_loc) {
    INST
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
    INST
    i.opcode = OP_RET;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
static void bcbuild_Jx(bytestream_t *bs, uint8_t opcode, uint32_t address, uint32_t *address_loc) {
    INST
    i.opcode = opcode;
    if (address_loc != NULL)
        *address_loc = bytestream_loc(bs);
    i.u.jf.address = address;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
    if (address_loc != NULL)
        *address_loc += (uint32_t)((uint8_t*)&i.u.frame.size - (uint8_t*)&i);
}

/*----------------------------------------------------------------------*/
void bcbuild_J(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    bcbuild_Jx(bs, OP_J, address, address_loc);
}

/*----------------------------------------------------------------------*/
void bcbuild_JT(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    bcbuild_Jx(bs, OP_JT, address, address_loc);
}

/*----------------------------------------------------------------------*/
void bcbuild_JF(bytestream_t *bs, uint32_t address, uint32_t *address_loc) {
    bcbuild_Jx(bs, OP_JF, address, address_loc);
}

/*----------------------------------------------------------------------*/
void bcbuild_PUSH_SI32(bytestream_t *bs, int32_t value) {
    INST
    i.opcode = OP_PUSH;
    i.subtype = OP_ST_SI32;
    i.u.push_si32.value = value;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st) {
    INST
    i.opcode = OP_ADD;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st) {
    INST
    i.opcode = OP_SUB;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st) {
    INST
    i.opcode = OP_MUL;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}

/*----------------------------------------------------------------------*/
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st) {
    INST
    i.opcode = OP_DIV;
    i.subtype = st;
    bytestream_pushn(bs, &i, sizeof(instruction_t));
}