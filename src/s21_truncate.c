#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_big_decimal big_value = s21_to_big(value);
  int scale = s21_get_scale(big_value);
  int sign = s21_get_sign(big_value);
  while (scale > 0) {
    s21_div10(&big_value);
    scale--;
  }
  s21_set_sign(&big_value, sign);
  s21_set_scale(&big_value, 0);
  *result = s21_to_dec(big_value);
  return 0;
}