#include <stdio.h>
#include <math.h>
#include <float.h>

const char *show_classification(double x) {
  switch(fpclassify(x)) {
    case FP_INFINITE:  return "Inf";
    case FP_NAN:       return "NaN";
    case FP_NORMAL:    return "normal";
    case FP_SUBNORMAL: return "subnormal";
    case FP_ZERO:      return "zero";
    default:           return "unknown";
  }
}

int main() {
    const char *classification;
    const unsigned int n = 5;
    double numbers[n];
    numbers[0] = 0;
    numbers[1] = FLT_MIN;
    numbers[2] = INFINITY;
    numbers[3] = NAN;
    numbers[4] = DBL_MIN / 2.0;
    for (unsigned int i = 0; i < n; i++) {
        printf("%e is %s\n", numbers[i], show_classification(numbers[i]));
    }
}
