#include<stdio.h>
#include<stdlib.h>

void f(void);

typedef void (func)(void);

static func * func_arr[3];

void one() {
    puts("1");
}

void two() {
    puts("2");
}

void three() {
    puts("3");
}


void invoke(unsigned int i) {
    (*func_arr[i])();
}

int main() {
    func_arr[0] = &one;
    func_arr[1] = &two;
    func_arr[2] = &three;

    for (unsigned int i = 0; i < 3; i++) {
        invoke(i);
    }
}
