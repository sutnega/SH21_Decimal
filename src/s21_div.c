#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;

  if (!result) {
    status = ERROR;
  } else {
    // ошибка деления на 0
    if (s21_is_zero_dec(value_2)) {  //(ERROR_ZERO_DIV | нужен обрабочик
                                     // ретерн-ошибок ??? не нужен
      status = ERROR_ZERO_DIV;
    }
    if (s21_is_zero_dec(value_1)) {  // если делитель 0
      s21_big_decimal zero = s21_new_decimal();
      s21_set_sign(&zero, 0);
      s21_set_scale(&zero, 0);
      *result = s21_to_dec(zero);
      status = OK;
    }

    s21_big_decimal big_value_1 = s21_to_big(value_1);
    s21_big_decimal big_value_2 = s21_to_big(value_2);

    big_value_1.bits[7] = 0;  // очищаем служебные поля
    big_value_2.bits[7] = 0;

    s21_big_decimal quotient = s21_new_decimal();
    s21_big_decimal remainder =
        s21_mod(big_value_1, big_value_2, &quotient, BIT_MODE_BIG);

    int sign =
        s21_get_sign(big_value_1) ^
        s21_get_sign(big_value_2);  // исключающее или, одинаковость знаков
    int scale = s21_get_scale(big_value_1) - s21_get_scale(big_value_2);

    // уточнение дробной части (умножаем остаток на 10^n, делим, увеличиваем
    // scale
    while (!s21_is_zero(remainder) &&
           s21_get_high_bit(quotient, BIT_MODE_BIG) < BIT_MODE_DEFAULT &&
           scale < 28) {
      scale++;
      s21_mul10(&remainder, BIT_MODE_BIG);
      s21_big_decimal temp = s21_new_decimal();
      remainder = s21_mod(remainder, big_value_2, &temp, BIT_MODE_BIG);
      s21_bsum(quotient, temp, &quotient, BIT_MODE_BIG);
    }

    // округление: если остаток >= половины делителя
    s21_big_decimal half_divisor = big_value_2;
    s21_right_shift(&half_divisor, 1);
    if (s21_is_equal_big(remainder, half_divisor, BIT_MODE_BIG)) {
      // четная ли целая часть?
      s21_big_decimal last_digit = s21_new_decimal();
      s21_mod(quotient, s21_pow10(1), &last_digit, BIT_MODE_DEFAULT);
      if (last_digit.bits[0] % 2 != 0) s21_bsum_unit(quotient, &quotient);
    } else if (!s21_is_less_ext(remainder, half_divisor, BIT_MODE_BIG)) {
      s21_bsum_unit(quotient, &quotient);
    }

    if (s21_get_high_bit(quotient, BIT_MODE_BIG) > BIT_MODE_DEFAULT) {
      status = sign ? ERROR_OVERFLOW_NEG : ERROR_OVERFLOW;
    } else {
      s21_set_sign(&quotient, sign);
      s21_set_scale(&quotient, scale < 0 ? 0 : scale);
      if (s21_is_zero(quotient)) s21_set_sign(&quotient, 0);
      *result = s21_to_dec(quotient);
    }
  }
  return status;
}