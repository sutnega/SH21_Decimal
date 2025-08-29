#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = OK;
  if (!dst) {
    status = ERROR_CONVERTATION;
  } else {
    s21_big_decimal result = s21_new_decimal();

    // ставим модуль числу
    if (src < 0) {
      result.bits[0] =
          (unsigned int)(-(long long)src);  // Безопасный модуль при минимальном
                                            // int, иначе не тесты не покрывают
      s21_set_sign(&result, 1);
    } else {
      result.bits[0] = (unsigned int)src;
      s21_set_sign(&result, 0);
    }

    s21_set_scale(&result, 0);

    *dst = s21_to_dec(result);
  }
  return status;
}