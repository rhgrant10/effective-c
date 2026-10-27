#include<stdio.h>
#include<stdlib.h>

int main() {
  if (puts("Hello, world!") == EOF) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
