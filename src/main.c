#include "bass.h"
#include "allocator/linear_allocator.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/
int main(int argc, char **argv) {
    uint8_t memory[1024*1024];
    allocator_t allocator = make_linear_allocator(memory, sizeof(memory)/sizeof(memory[0]));
    bass_state_t *B;

    /* read source file */
    if (argc != 2) {
        printf("usage: basic <file.bass>\n");
        return -1;
    }

    B = bass_open(allocator);
    return bass_do_file(B, argv[1]);
}
