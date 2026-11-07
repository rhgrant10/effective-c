#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

typedef struct {
    int value;
    unsigned int expected;
} test_case;


unsigned int absolute_value(int a) {
    if (a >= 0) {
        return (unsigned)a;
    }
    return -(unsigned)a;
}


int main() {
    test_case test_cases[] = {
        {0, 0u},
        {-1, 1u},
        {1, 1u},
        {-INT_MAX, (unsigned)INT_MAX},
        {INT_MIN, (unsigned)INT_MAX + 1u},
    };

    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    bool all_pass = true;
    for (int i = 0; i < num_cases; i++) {
        unsigned int actual = absolute_value(test_cases[i].value);
        if (actual != test_cases[i].expected) {
            all_pass = false;
            printf("FAIL absolute_value(%d): want %u, got %u\n", test_cases[i].value, test_cases[i].expected, actual);
        } else {
            printf("PASS absolute_value(%d) == %u\n", test_cases[i].value, test_cases[i].expected);
        }
    }

    if (all_pass) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
