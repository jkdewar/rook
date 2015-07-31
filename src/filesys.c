#include "filesys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------*/
int read_file(const char *file_name, char **contents, size_t *size) {
    size_t file_size;
    size_t read_size;

    FILE* file = fopen(file_name, "rb");
    if (!file)
        return 0;
    fseek(file , 0 , SEEK_END);
    file_size = ftell(file);
    rewind(file);
    *contents = malloc(file_size + 1);
    read_size = fread(*contents, 1, file_size, file);
    fclose(file);
    contents[file_size] = '\0';
    if (read_size != file_size) {
        free(*contents);
        *contents = NULL;
        return 0;
    }
    return 1;
}

/*----------------------------------------------------------------------*/
int write_file(const char *file_name, const char *contents, size_t size) {
    size_t result;

    FILE* file = fopen(file_name, "wb");
    if (!file)
        return 0;
    result = fwrite(contents, 1, size, file);
    fclose(file);
    if (result != size)
        return 0;
    return 1;
}
