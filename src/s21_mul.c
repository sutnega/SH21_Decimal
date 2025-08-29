#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;

  if (!result) {
    status = ERROR;
  } else {
    s21_big_decimal big_value_1 = s21_to_big(value_1);
    s21_big_decimal big_value_2 = s21_to_big(value_2);
    s21_big_decimal res = s21_new_decimal();

    int sign = s21_get_sign(big_value_1) ^ s21_get_sign(big_value_2);
    int scale = s21_get_scale(big_value_1) + s21_get_scale(big_value_2);

    big_value_1.bits[7] = 0;  // очищаем служебные поля
    big_value_2.bits[7] = 0;

    // умножение в столбик
    s21_big_decimal buffer = big_value_1;
    int high_bit = s21_get_high_bit(big_value_2, BIT_MODE_DEFAULT);
    for (int i = 0; i <= high_bit && status == OK; i++) {
      if (s21_get_bit(big_value_2, i))
        status = s21_bsum(res, buffer, &res, BIT_MODE_BIG);
      if (status == OK) status = s21_left_shift(&buffer, 1, BIT_MODE_BIG);
    }

    // нормализация (если переполнение)
    int denorm = s21_get_high_bit(res, BIT_MODE_BIG) - BIT_MODE_DEFAULT;
    s21_big_decimal remainder = s21_new_decimal();

    while (scale > 0 && denorm > 0) {
      remainder = s21_mod(res, s21_pow10(1), &res, BIT_MODE_BIG);
      scale--;
      denorm = s21_get_high_bit(res, BIT_MODE_BIG) - BIT_MODE_DEFAULT;
    }

    if (denorm <= 0) {
      // банковское округление
      s21_big_decimal last_digit = s21_new_decimal();
      s21_mod(res, s21_pow10(1), &last_digit, BIT_MODE_DEFAULT);
      if (remainder.bits[0] == 5 && s21_is_zero(remainder)) {
        if (last_digit.bits[0] % 2 != 0) s21_bsum_unit(res, &res);
      } else if (remainder.bits[0] > 5) {
        s21_bsum_unit(res, &res);
      }

      // переполнение
      if (status == OK && scale > 28)
        status = s21_decrease_scale(&res, scale - 28);

      if (status == OK) {
        if (s21_is_zero_big_dec(res)) scale = 0;
        s21_set_sign(&res, sign);
        s21_set_scale(&res, scale);
        *result = s21_to_dec(res);
      }
    } else {
      status = sign ? ERROR_OVERFLOW_NEG : ERROR_OVERFLOW;
    }
  }
  return status;
}