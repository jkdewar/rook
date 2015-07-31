#pragma once
#include "allocator/allocator.h"

typedef struct rook_state_t rook_state_t;

rook_state_t *rook_open(allocator_t allocator);
void rook_close(rook_state_t *R);
int rook_do_file(rook_state_t *R, const char *file_name);
int rook_get_error(rook_state_t *R);
const char *rook_get_error_str(rook_state_t *R);
