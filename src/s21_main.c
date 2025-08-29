#include "s21_decimal.h"

// Преобразование между decimal и big_decimal.
s21_big_decimal s21_to_big(s21_decimal value) {
  s21_big_decimal buf = {
      {value.bits[0], value.bits[1], value.bits[2], 0, 0, 0, 0, value.bits[3]}};
  return buf;
}

s21_decimal s21_to_dec(s21_big_decimal value) {
  s21_decimal buf = {
      {value.bits[0], value.bits[1], value.bits[2], value.bits[7]}};
  return buf;
}

// Getters & Setters.
int s21_get_bit(s21_big_decimal value, int pos) {
  return 1u & (value.bits[pos / 32] >> pos % 32);
}

void s21_set_bit(s21_big_decimal *value, int pos, int bit) {
  if (bit)
    value->bits[pos / 32] |= 1u << pos % 32;
  else if (!bit)
    value->bits[pos / 32] &= ~(1u << pos % 32);
}

int s21_get_sign(s21_big_decimal value) { return s21_get_bit(value, 255); }

s21_big_decimal *s21_set_sign(s21_big_decimal *value, int sign) {
  s21_set_bit(value, 255, sign);
  return value;
}

int s21_get_scale(s21_big_decimal value) {
  return (char)(value.bits[7] >> 16);  // Масштаб занимает биты 16-23 - это
                                       // ровно 8 бит = 1 байт = char
}

s21_big_decimal *s21_set_scale(s21_big_decimal *value, char scale) {
  int sign = s21_get_sign(*value);
  value->bits[7] = scale << 16;
  s21_set_sign(value, sign);
  return value;
}

int s21_align_scale(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int status = OK;
  int scale1 = s21_get_scale(*value_1);
  int scale2 = s21_get_scale(*value_2);
  int diff = scale1 - scale2;

  if (diff > 0) {
    // value_2 надо догнать вверх
      status = s21_increase_scale(value_2, diff);
      if (status == ERROR_OVERFLOW) {
    // fallback  - уменьшаем scale value_1
      status = s21_decrease_scale(value_1, diff);
      }
  } else if (diff < 0) {
    diff = -diff;
    status = s21_increase_scale(value_1, diff);

    if (status == ERROR_OVERFLOW) {
      status = s21_decrease_scale(value_2, diff);
    }
  }
  return status;
}

// пока только на 1 степерь за раз // а больше не нужно, если цикл
int s21_increase_scale(s21_big_decimal *value, int shift) {
  int status = OK;
  int scale = s21_get_scale(*value);

  for (int i = 0; i < shift && status == OK; ++i) {
    if (scale + 1 > 28 || s21_mul10(value, BIT_MODE_DEFAULT)) {
      status = ERROR_OVERFLOW;
    } else {
      scale++;
    }
  }

  if (status == OK) {
    s21_set_scale(value, scale);
  }

  return status;
}

int s21_decrease_scale(s21_big_decimal *value, int shift) {
  int status = OK;
  int scale = s21_get_scale(*value);
  if (scale - shift < 0) {
    status = ERROR_UNDERFLOW;
  } else {
    s21_decimal val = s21_to_dec(*value);

    s21_decimal divisor = s21_to_dec(s21_pow10(shift));  // Делитель 10^шифт
    s21_decimal result;
    status = s21_div(val, divisor, &result);

    if (status == OK) {
      s21_decimal check;
      s21_mul(result, divisor, &check);  // тестим потери точности умножением

      if (!s21_is_equal(val, check)) {
        status = ERROR_UNDERFLOW;
      } else {
        *value = s21_to_big(result);  // стандарт
        s21_set_scale(value, scale - shift);
        s21_set_sign(value, s21_get_sign(*value));
      }
    }
  }
  return status;
}

int s21_mul10(s21_big_decimal *value, int mode) {
  int status = OK;
  s21_big_decimal buf1 = *value, buf2 = *value;
  if (!s21_left_shift(&buf1, 1, mode) && !s21_left_shift(&buf2, 3, mode)) {
    s21_big_decimal buf3 = *value;
    if (!(status = s21_bsum(buf1, buf2, &buf3, mode))) *value = buf3;
  } else
    status = ERROR_OVERFLOW;
  return status;
}

// Тупо делит и выкидывает дробную часть
void s21_div10(s21_big_decimal *value) {
  s21_big_decimal result = s21_new_decimal();
  s21_mod(*value, s21_pow10(1), &result, BIT_MODE_BIG);
  *value = result;
}

s21_big_decimal s21_pow10(int scale) {
  s21_big_decimal result = s21_new_decimal();
  result.bits[0] = 1;
  for (int i = 0; i < scale; i++) s21_mul10(&result, BIT_MODE_BIG);
  return result;
}

int s21_is_less_ext(s21_big_decimal value_1, s21_big_decimal value_2,
                    int mode) {
  int res = 0;
  for (int i = mode; i >= 0 && !res; i--)
    res = s21_get_bit(value_2, i) - s21_get_bit(value_1, i);
  return res > 0 ? 1 : 0;
}

// binary
// Побитовое сложение двух чисел
int s21_bsum(s21_big_decimal value1, s21_big_decimal value2,
             s21_big_decimal *result, int high_bit) {
  int bit = 0;
  for (int i = 0; i <= high_bit; i++) {
    int sum = s21_get_bit(value1, i) + s21_get_bit(value2, i) + bit;
    s21_set_bit(result, i, sum % 2);
    bit = sum >= 2 ? 1 : 0;
  }
  return bit;
}

void s21_bsub(s21_big_decimal value_1, s21_big_decimal value_2,
              s21_big_decimal *result, int mode) {
  s21_reset(result);
  if (!s21_is_equal_big(value_1, value_2, mode)) {
    int high_bit = s21_get_high_bit(value_1, mode);
    s21_big_decimal buf = value_2;
    s21_reverse(&buf);
    s21_bsum(value_1, buf, result, high_bit);
    s21_big_decimal unit = DEC_BIG_UNIT;
    s21_bsum(*result, unit, result, high_bit);
  }
}

s21_big_decimal s21_new_decimal() {
  s21_big_decimal new = {0};
  return new;
}
// Проверка всего s21_big_decimal на 0 (мантисса + bits[7]), вернёт 1 если число
// это нуль
int s21_is_zero(s21_big_decimal value) {
  int res = 1;
  for (int i = 0; i < 8 && res; i++) res = value.bits[i] == 0;
  return res;
}
// Проверка мантиссы s21_decimal на 0 (bits[0-3])
int s21_is_zero_dec(s21_decimal value) {
  int res = 1;
  for (int i = 0; i < 3 && res; i++) res = value.bits[i] == 0;
  return res;
}
// Проверка мантиссы s21_big_decimal на 0 (bits[0-7]), вернёт 1 если везде нуль
int s21_is_zero_big_dec(s21_big_decimal value) {
  int res = 1;
  for (int i = 0; i < 7 && res; i++) res = value.bits[i] == 0;
  return res;
}
// Обнуляет s21_big_decimal
void s21_reset(s21_big_decimal *value) {
  for (int i = 0; i < 8; i++) value->bits[i] = 0;
}

// Заменяет 0 на 1, 1 на 0
void s21_reverse(s21_big_decimal *value) {
  for (int i = 0; i < 7; i++) value->bits[i] = ~value->bits[i];
}

// Берет старшее значение
int s21_get_high_bit(s21_big_decimal value, int mode) {
  int bit = mode;
  while (bit >= 0 && !s21_get_bit(value, bit)) bit--;
  return bit;
}
// Сдвиг битов влево на shift и проверка переполнения
int s21_left_shift(s21_big_decimal *value, int shift, int mode) {
  int i = mode;
  int status =
      s21_get_high_bit(*value, mode) + shift <= mode ? OK : ERROR_OVERFLOW;
  for (; i - shift >= 0; i--)
    s21_set_bit(value, i, s21_get_bit(*value, i - shift));
  for (; i >= 0; i--) s21_set_bit(value, i, 0);
  return status;
}

void s21_right_shift(s21_big_decimal *value, int shift) {
  int i = 0;
  for (; i + shift <= BIT_MODE_BIG; i++)
    s21_set_bit(value, i, s21_get_bit(*value, i + shift));
  for (; i <= BIT_MODE_BIG; i++) s21_set_bit(value, i, 0);
}

// Нахождение остатка от деления
s21_big_decimal s21_mod(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result, int mode) {
  s21_reset(result);
  s21_big_decimal remainder = s21_new_decimal();
  int high_bit = s21_get_high_bit(value_1, mode);

  // деление в столбик
  while (high_bit >= 0) {
    s21_left_shift(&remainder, 1, mode);
    s21_set_bit(&remainder, 0, s21_get_bit(value_1, high_bit));
    if (s21_is_less_ext(remainder, value_2, mode)) {
      s21_left_shift(result, 1, mode);
      s21_set_bit(result, 0, 0);
    } else {
      s21_bsub(remainder, value_2, &remainder, mode);
      s21_left_shift(result, 1, mode);
      s21_set_bit(result, 0, 1);
    }
    high_bit--;
  }

  return remainder;
}

//+1
int s21_bsum_unit(s21_big_decimal value1, s21_big_decimal *result) {
  s21_big_decimal unit = DEC_BIG_UNIT;
  return s21_bsum(value1, unit, result, BIT_MODE_DEFAULT);
}

// Сравнение двух чисел и возвращает меньше -1, равно 0, больше 1
int s21_comparison(s21_decimal value_1, s21_decimal value_2) {
  if (s21_is_zero_dec(value_1) && s21_is_zero_dec(value_2))
    return 0;  // если оба нули

  s21_big_decimal big_value_1 = s21_to_big(value_1);
  s21_big_decimal big_value_2 = s21_to_big(value_2);

  int sign1 = s21_get_sign(big_value_1);
  int sign2 = s21_get_sign(big_value_2);

  if (sign1 != sign2) {  // если разные знаки
    return sign1 ? -1 : 1;
  }

  s21_align_scale(&big_value_1, &big_value_2);  // выравниваем

  // сравниваем мантиссы от старших к младшим
  for (int i = 2; i >= 0; i--) {
    if (big_value_1.bits[i] != big_value_2.bits[i]) {
      int result = (big_value_1.bits[i] > big_value_2.bits[i]) ? 1 : -1;
      return sign1 ? -result : result;
    }
  }

  return 0;
}

// Сравнивание бигдецимал
int s21_is_equal_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     int mode) {
  (void)mode;
  int res = 1;
  for (int i = 0; (i < 7) && res; i++) res = value_1.bits[i] == value_2.bits[i];
  return res;
}