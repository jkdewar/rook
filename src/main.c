#include "rook.h"
#include "allocator/linear_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void test();

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    uint8_t memory[1024 * 1024];
    allocator_t allocator = make_linear_allocator(memory, sizeof(memory)/sizeof(memory[0]));
    char *file_name;
    rook_state_t *R;

    if (argc != 2) {
        printf("usage: rook <file.rook>\n");
        return -1;
    }
    file_name = argv[1];
    R = rook_open(allocator);
    rook_do_file(R, file_name);
    rook_close(R);
    return 0;
}
