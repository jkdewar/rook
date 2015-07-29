#pragma once
#include <stddef.h>

int read_text_file(const char *file_name, char **contents);
int write_text_file(const char *file_name, const char *contents);
int write_binary_file(const char *file_name, const char *contents, size_t size);
