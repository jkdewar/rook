#include "bytecode.h"
#include <stdio.h>
#include <string.h>

#define INST(OP) instruction_t i;  memset(&i, 0xff, sizeof(instruction_t)); i.opcode = OP
#define PUSH() bytestream_pushn(bs, &i, sizeof(instruction_t));


/*----------------------------------------------------------------------*/
void bcbuild_STORE(bytestream_t *bs, uint32_t size, int32_t stack_pos) {
    INST(OP_STORE);
    i.u.store.size = size;
    i.u.store.stack_pos = stack_pos;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_LOAD(bytestream_t *bs, uint32_t size, int32_t stack_pos) {
    INST(OP_LOAD);
    i.u.load.size = size;
    i.u.load.stack_pos = stack_pos;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_FRAME(bytestream_t *bs, uint32_t size, uint32_t *size_where) {
    INST(OP_FRAME);
    i.u.frame.size = size;
    if (size_where != NULL)
        *size_where = bytestream_where(bs);
    PUSH();
    if (size_where != NULL)
        *size_where += (uint32_t)((uint8_t*)&i.u.frame.size - (uint8_t*)&i);
}

/*----------------------------------------------------------------------*/
void bcbuild_CALL(bytestream_t *bs, uint32_t address) {
    INST(OP_CALL);
    i.u.call.address = address;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_RET(bytestream_t *bs) {
    INST(OP_RET);
    PUSH();
}

/*----------------------------------------------------------------------*/
static void bcbuild_Jx(bytestream_t *bs, uint8_t opcode, uint32_t address, uint32_t *address_where) {
    INST(opcode);
    if (address_where != NULL)
        *address_where = bytestream_where(bs);
    i.u.jf.address = address;
    PUSH();
    if (address_where != NULL)
        *address_where += (uint32_t)((uint8_t*)&i.u.frame.size - (uint8_t*)&i);
}

/*----------------------------------------------------------------------*/
void bcbuild_J(bytestream_t *bs, uint32_t address, uint32_t *address_where) {
    bcbuild_Jx(bs, OP_J, address, address_where);
}

/*----------------------------------------------------------------------*/
void bcbuild_JT(bytestream_t *bs, uint32_t address, uint32_t *address_where) {
    bcbuild_Jx(bs, OP_JT, address, address_where);
}

/*----------------------------------------------------------------------*/
void bcbuild_JF(bytestream_t *bs, uint32_t address, uint32_t *address_where) {
    bcbuild_Jx(bs, OP_JF, address, address_where);
}

/*----------------------------------------------------------------------*/
void bcbuild_PUSH_SI32(bytestream_t *bs, int32_t value) {
    INST(OP_PUSH);
    i.subtype = OP_ST_SI32;
    i.u.push_si32.value = value;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_PUSH_F(bytestream_t *bs, float value) {
    INST(OP_PUSH);
    i.subtype = OP_ST_F;
    i.u.push_f.value = value;
    PUSH();
}

/*----------------------------------------------------------------------*/
void bcbuild_PUSH_D(bytestream_t *bs, double value) {
    INST(OP_PUSH);
    i.subtype = OP_ST_D;
    i.u.push_d.value = value;
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
