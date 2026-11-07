#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

typedef struct {
    int value;
    int expected;
} test_case;


int absolute_value(int a) {
    if (a >= 0) {
        return a;
    }
    return -(unsigned)a;
}


int main() {
    test_case test_cases[] = {
        {0, 0},
        {-1, 1},
        {1, 1},
        {-INT_MAX, INT_MAX},
        {INT_MIN, INT_MIN},  // correct because -INT_MIN = INT_MAX + 1
    };

    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    bool all_pass = true;
    for (int i = 0; i < num_cases; i++) {
        int actual = absolute_value(test_cases[i].value);
        if (actual != test_cases[i].expected) {
            all_pass = false;
            printf("FAIL absolute_value(%d): want %d, got %d\n", test_cases[i].value, test_cases[i].expected, actual);
        } else {
            printf("PASS absolute_value(%d) == %d\n", test_cases[i].value, test_cases[i].expected);
        }
    }

    if (all_pass) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
