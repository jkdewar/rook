#include "bytecode.h"
#include <stdio.h>
#include <string.h>

#define INST(OP) instruction_t i;  memset(&i, 0xff, sizeof(instruction_t)); i.opcode = OP
#define SUBTYPE(ST) i.subtype = ST
#define PUSH() bytestream_pushn(bs, &i, sizeof(instruction_t));


/*----------------------------------------------------------------------*/
void bcbuild_STORE(bytestream_t *bs, uint32_t size, int32_t stack_pos) {
    INST(OP_STORE);
    i.u.store.size = size;
    i.u.store.stack_pos = stack_pos;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_FRAME(bytestream_t *bs, uint32_t size, uint32_t *size_loc) {
    INST(OP_FRAME);
    i.u.frame.size = size;
    if (size_loc != NULL)
        *size_loc = bytestream_loc(bs);
    bytestream_pushn(bs, &i, sizeof(instruction_t));
    if (size_loc != NULL)
        *size_loc += (uint32_t)((uint8_t*)&i.u.frame.size - (uint8_t*)&i);
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_RET(bytestream_t *bs) {
    INST(OP_RET);
    PUSH();
}

/*----------------------------------------------------------------------*/
static void bcbuild_Jx(bytestream_t *bs, uint8_t opcode, uint32_t address, uint32_t *address_loc) {
    INST(opcode);
    if (address_loc != NULL)
        *address_loc = bytestream_loc(bs);
    i.u.jf.address = address;
    PUSH();
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
    INST(OP_PUSH);
    i.subtype = OP_ST_SI32;
    i.u.push_si32.value = value;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_ADD);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_SUB);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_MUL);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_DIV);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_TL(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_TL);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_TLE(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_TLE);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_TE(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_TE);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_TGE(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_TGE);
    i.subtype = st;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_TG(bytestream_t *bs, opcode_subtype_t st) {
    INST(OP_TG);
    i.subtype = st;
    PUSH();
}
