#include "rook.h"
#include "allocator/linear_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    uint8_t memory[1024*1024];
    allocator_t allocator = make_linear_allocator(memory, sizeof(memory)/sizeof(memory[0]));
    rook_state_t *R;

    /* read source file */
    if (argc != 2) {
        printf("usage: rook <file.rook>\n");
        return -1;
    }

    R = rook_open(allocator);
    rook_do_file(R, argv[1]);
    rook_close(R);
}
