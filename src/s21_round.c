#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int status = OK;

  if (!result) {
    status = ERROR;
  } else {
    s21_decimal truncated;  // берем целую часть
    status = s21_truncate(value, &truncated);

    if (status == OK) {
      s21_decimal remainder;  // берем дробную
      status = s21_sub(value, truncated, &remainder);

      s21_big_decimal big_half = {{5, 0, 0, 0}};  // вводим 0.5
      s21_set_scale(&big_half, 1);
      s21_decimal half = s21_to_dec(big_half);

      if (status == OK && s21_is_greater_or_equal(remainder, half)) {
        s21_decimal unit = DEC_UNIT;

        s21_big_decimal big_value = s21_to_big(value);
        if (s21_get_sign(big_value)) {
          status = s21_sub(truncated, unit,
                           result);  // округляем в минус для отрицательного
        } else {
          status = s21_add(truncated, unit,
                           result);  // округляем в плюс для положительного
        }
      } else {
        *result = truncated;  // округляем вниз если 0.5
      }
    }
  }

  return status;
}