#include "bytecode.h"

#define PUSH_OP(OP) bytestream_push8(bs, (uint8_t)(OP))
#define PUSH_ST(ST) bytestream_push8(bs, (uint8_t)(ST))
#define PUSH_8(X) bytestream_push8(bs, X)
#define PUSH_16(X) bytestream_push16(bs, X)
#define PUSH_32(X) bytestream_push32(bs, X)
#define PUSH_64(X) bytestream_push64(bs, X)

/*----------------------------------------------------------------------*/
void bcbuild_RET(bytestream_t *bs) {
    PUSH_OP(OP_RET);
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
    PUSH_OP(OP_PUSH);
    PUSH_ST(OP_ST_SI32);
    PUSH_32(value);
}

/*----------------------------------------------------------------------*/
void bcbuild_ADD(bytestream_t *bs, opcode_subtype_t st) {
    PUSH_OP(OP_ADD);
    PUSH_ST(st);
}

/*----------------------------------------------------------------------*/
void bcbuild_SUB(bytestream_t *bs, opcode_subtype_t st) {
    PUSH_OP(OP_SUB);
    PUSH_ST(st);
}

/*----------------------------------------------------------------------*/
void bcbuild_MUL(bytestream_t *bs, opcode_subtype_t st) {
    PUSH_OP(OP_MUL);
    PUSH_ST(st);
}

/*----------------------------------------------------------------------*/
void bcbuild_DIV(bytestream_t *bs, opcode_subtype_t st) {
    PUSH_OP(OP_DIV);
    PUSH_ST(st);
}
