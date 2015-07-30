#pragma once
#include "allocator/allocator.h"

typedef struct bass_state_t bass_state_t;

bass_state_t *bass_open(allocator_t allocator);
void bass_close(bass_state_t *B);
int bass_do_file(bass_state_t *B, const char *file_name);
int bass_get_error(bass_state_t *B);
const char *bass_get_error_str(bass_state_t *B);
