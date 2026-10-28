#include <stdio.h>
#include <stdlib.h>

static unsigned int counter = 0;

void increment(void) {
  counter++;
}

unsigned int retrieve() {
  return counter;
}

int main() {
  for (int i = 0; i < 5; i++) {
    increment();
    unsigned int curr_count = retrieve();
    printf("%d ", curr_count);
  }
  return EXIT_SUCCESS;
}

