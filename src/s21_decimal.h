#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define OK 0
#define ERROR_CONVERTATION 1
#define ERROR_OVERFLOW 1
#define ERROR_UNDERFLOW 2
#define ERROR_OVERFLOW_NEG 2
#define ERROR_ZERO_DIV 3
#define ERROR 4

#define MAX_NUM 79228162514264337593543950335.0
#define MIN_NUM -79228162514264337593543950335.0

#define DEC_UNIT   \
  {                \
    { 1, 0, 0, 0 } \
  }

#define DEC_BIG_UNIT           \
  {                            \
    { 1, 0, 0, 0, 0, 0, 0, 0 } \
  }

#define BIT_MODE_DEFAULT 95
#define BIT_MODE_BIG 223

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
} s21_big_decimal;

// Convert
s21_big_decimal s21_to_big(s21_decimal value);
s21_decimal s21_to_dec(s21_big_decimal value);

// Setters & getters
int s21_get_sign(s21_big_decimal d);
s21_big_decimal *s21_set_sign(s21_big_decimal *value, int sign);
int s21_get_scale(s21_big_decimal d);
s21_big_decimal *s21_set_scale(s21_big_decimal *value, char scale);

int s21_get_bit(s21_big_decimal d, int position);
void s21_set_bit(s21_big_decimal *d, int position, int bit);

int s21_is_zero(s21_big_decimal d);
int s21_is_zero_dec(s21_decimal d);
int s21_is_zero_big_dec(s21_big_decimal value);

// Helpers
int s21_is_less_ext(s21_big_decimal value_1, s21_big_decimal value_2, int mode);
void s21_reset(s21_big_decimal *value);
int s21_mul10(s21_big_decimal *value, int mode);
void s21_div10(s21_big_decimal *value);
s21_big_decimal s21_pow10(int scale);
int s21_increase_scale(s21_big_decimal *value, int shift);
int s21_decrease_scale(s21_big_decimal *value, int shift);
int s21_align_scale(s21_big_decimal *value_1, s21_big_decimal *value_2);
int s21_is_equal_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     int mode);
s21_big_decimal s21_mod(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result, int mode);
int s21_comparison(s21_decimal value_1, s21_decimal value_2);

// BINARY
int s21_get_high_bit(s21_big_decimal value, int mode);
void s21_reverse(s21_big_decimal *value);
int s21_left_shift(s21_big_decimal *value, int shift, int mode);
void s21_right_shift(s21_big_decimal *value, int shift);
int s21_bsum(s21_big_decimal value1, s21_big_decimal value2,
             s21_big_decimal *result, int high_bit);
void s21_bsub(s21_big_decimal value_1, s21_big_decimal value_2,
              s21_big_decimal *result, int mode);
s21_big_decimal s21_new_decimal();
int s21_bsum_unit(s21_big_decimal value1, s21_big_decimal *result);

// Arithmetic
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparsion
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Сonversion
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Other
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif