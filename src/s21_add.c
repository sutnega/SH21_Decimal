#include "s21_decimal.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;

  if (!result) {
    status = ERROR;
  } else {
    s21_big_decimal big_value_1 = s21_to_big(value_1);
    s21_big_decimal big_value_2 = s21_to_big(value_2);
    s21_big_decimal res = {0};

    int sign1 = s21_get_sign(big_value_1);
    int sign2 = s21_get_sign(big_value_2);

    s21_align_scale(&big_value_1, &big_value_2);
    int scale = s21_get_scale(big_value_1);

    if (scale > 28) {
      status = ERROR_OVERFLOW;
    } else {
      if (sign1 == sign2) {
        status = s21_bsum(big_value_1, big_value_2, &res, BIT_MODE_DEFAULT);
        if (status == ERROR_OVERFLOW && scale > 0) {
          s21_big_decimal temp1 = big_value_1;
          s21_big_decimal temp2 = big_value_2;
          while (scale > 0 && s21_decrease_scale(&temp1, 1) == OK &&
                 s21_decrease_scale(&temp2, 1) == OK) {
            scale--;
            status = s21_bsum(temp1, temp2, &res, BIT_MODE_DEFAULT);
            if (status != ERROR_OVERFLOW) break;
          }
          if (status == ERROR_OVERFLOW) {
            s21_reset(&res);
          }
        }
        s21_set_sign(&res, sign1);
      } else if (sign1 < sign2) {
        if (s21_is_less_ext(big_value_2, big_value_1, BIT_MODE_DEFAULT)) {
          s21_bsub(big_value_1, big_value_2, &res, BIT_MODE_DEFAULT);
        } else {
          s21_bsub(big_value_2, big_value_1, &res, BIT_MODE_DEFAULT);
          s21_set_sign(&res, 1);
        }
      } else {
        if (s21_is_less_ext(big_value_1, big_value_2, BIT_MODE_DEFAULT)) {
          s21_bsub(big_value_2, big_value_1, &res, BIT_MODE_DEFAULT);
        } else {
          s21_bsub(big_value_1, big_value_2, &res, BIT_MODE_DEFAULT);
          s21_set_sign(&res, 1);
        }
      }

      if (status == OK) {
        s21_set_scale(&res, scale);
        if (s21_is_zero(res)) {
          s21_set_sign(&res, 0);
        }
        *result = s21_to_dec(res);
      }
    }
  }

  return status;
}