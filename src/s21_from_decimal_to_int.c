#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = OK;

  if (!dst) {
    status = ERROR_CONVERTATION;
  } else {
    s21_big_decimal value = s21_to_big(src);
    int sign = s21_get_sign(value);
    int scale = s21_get_scale(value);

    // выкидываем дробную часть
    while (scale > 0) {
      s21_div10(&value);  // берем ТОЛЬКО частное от /10
      scale--;
    }

    src = s21_to_dec(value);

    // если оверфлоу
    status = src.bits[1] != 0 || src.bits[2] != 0 ||
             ((1u & (src.bits[0] >> 31)) && !sign);

    if (!status) *dst = sign ? -(int)src.bits[0] : (int)src.bits[0];
  }

  return status;
}