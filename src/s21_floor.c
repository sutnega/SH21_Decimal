#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = OK;
  s21_big_decimal buf = s21_to_big(value);
  status = s21_truncate(value, &value);
  s21_big_decimal tmp = s21_to_big(value);
  *result = s21_to_dec(tmp);
  s21_decimal unit = DEC_UNIT;
  if (s21_get_scale(buf) > 0 && !s21_is_zero_big_dec(buf) &&
      s21_get_sign(buf)) {
    status = s21_sub(*result, unit, result);
  }
  return status;
}
