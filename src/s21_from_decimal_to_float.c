#include <math.h>

#include "s21_decimal.h"

// короче decimal — это по сути: value = (sign ? -1 : 1) × mantissa × 10^-scale
// (нужен float)

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = OK;

  if (!dst) {
    status = ERROR_CONVERTATION;
  } else {
    s21_big_decimal big_value = s21_to_big(src);  // стандарт
    int sign = s21_get_sign(big_value);
    int scale = s21_get_scale(big_value);

    double result = 0.0;  // double нужен для мантиссы

    for (int i = 0; i < 96; i++) {
      if (s21_get_bit(big_value, i)) {
        result += pow(2.0, i);  // сумма всех битов, как 2^i
      }
    }

    result = result / pow(10.0, scale);  // х**рим деление на 10^scale

    if (sign) {
      result *= -1.0;  // учет знака
    }

    *dst = (float)result;
  }

  return status;
}