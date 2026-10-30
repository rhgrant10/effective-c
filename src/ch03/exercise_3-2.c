#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int i = INT_MAX;
    float f = (float)i;
    printf("%i as float is %f\n", i, f);
    return EXIT_SUCCESS;
}
