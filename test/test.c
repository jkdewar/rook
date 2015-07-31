#include <stdio.h>

int main(int argc, char **argv) {
    int total = 0;
    int i;
    for (i = 1; i <= 100000000; ++i)
        total = total + i;
    printf("%d\n", total);
    return 0;
}
