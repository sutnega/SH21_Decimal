#include "s21_decimal.h"
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_comparison(value_1, value_2) > 0;
}