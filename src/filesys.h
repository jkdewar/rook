#pragma once
#include <stddef.h>

int read_file(const char *file_name, char **contents, size_t *size);
int write_file(const char *file_name, const char *contents, size_t size);
