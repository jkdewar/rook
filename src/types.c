#include "types.h"
#include <assert.h>

/*----------------------------------------------------------------------*/
int types_match(type_t *a, type_t *b) {
    if (a->tag != b->tag) {
        return 0;
    }
    /* both tags are equal... */
    if (a->tag == TTAG_BASIC_TYPE) {
        return a->u.basic_type == b->u.basic_type;
    } else if (a->tag == TTAG_POINTER) {
        return types_match(a->u.pointer_type, b->u.pointer_type);
    } /*else if (a->tag == TTAG_STRUCT) { TODO:jkd
    }*/
    assert(0); /* unreachable */
    return 0;
}
