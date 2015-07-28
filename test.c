#include <stdlib.h>

int main(int argc, char **argv) {
    int i;

    for (i = 0; i < 1000000; ++i) {
        malloc(1);
    }

    return 0;
}
