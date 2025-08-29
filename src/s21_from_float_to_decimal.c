#include <math.h>

#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = OK;

  if (!dst || isnan(src) || isinf(src) || fabsf(src) < 1e-28f ||
      fabsf(src) > 7.92281625e+28f) {
    status = ERROR_CONVERTATION;
  } else {
    s21_big_decimal value = s21_new_decimal();

    int sign = signbit(src);  // знак float 0 или 1 (нужен для set_sign в конце)
    float abs_src = fabsf(src);  // модуль float
    int scale = 0;

    while (fabsf(abs_src - floorf(abs_src)) > 1e-6f && scale < 28) {
      abs_src *= 10.0f;  //^модуль дробной части
      scale++;           //*10 пока не уйдет запятая
    }

    abs_src = roundf(abs_src);  // округляет до 7значного числа(по заданию)

    value.bits[0] = (unsigned int)abs_src;
    s21_set_scale(&value, scale);
    s21_set_sign(&value, sign);
    *dst = s21_to_dec(value);
  }

  return status;
}