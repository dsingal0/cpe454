#include "printu.h"

#include <stdarg.h>

#include "io.h"

#define LOWER_CASE 0
#define UPPER_CASE 1

#define PRINT_STRING(string) Puts(string)
#define PRINT_CHAR(character) Putc(character)

#define PRINT_NUMBER(type, base, upper_case) {        \
  {                                                   \
    auto value = va_arg(list, type);                  \
    if (value < 0) {                                  \
      PRINT_CHAR('-');                                \
    }                                                 \
    ToString(value, string_buffer, base, upper_case); \
    PRINT_STRING(string_buffer);                      \
  }                                                   \
}

template <typename T>
static char ToChar(T number, int upper_case) {
  if (number < 0) {
    number *= -1;
  }

  if (number > 15) {
    // error
    return '?';
  } else if (number < 10) {
    return ((char) number) + '0';
  } else if (upper_case) {
    return ((char) number) + 'A' - 10;
  } else {
    return ((char) number) + 'a' - 10;
  }
}

// doesn't include '-' for negative numbers
template <typename T>
static void ToString(T value, char* string, int base, int upper_case) {
  int num_digits = 0;
  if (!value) {
    num_digits = 1;
  } else {
    T value_copy = value;
    while (value_copy) {
      value_copy /= base;
      num_digits++;
    }
  }

  for (int i = 0; i < num_digits; i++) {
    char digit = ToChar(value % base, upper_case);
    value /= base;
    string[num_digits - 1 - i] = digit;
  }
  string[num_digits] = '\0';
}

// TODO what is this supposed to return?
int printu(const char* format, ...) {
  va_list list;
  va_start(list, format);
  int int_value, num_digits;
  long int long_int_value;
  long long int long_long_int_value;
  char string_buffer[64] = {0};

  while (*format) {
    if (*format == '%') {
      format++;
      switch (*format) {
        case '%':
          PRINT_CHAR('%');
          break;

        case 'i':
        case 'd':
          PRINT_NUMBER(int, 10, LOWER_CASE);
          break;

        case 'u':
          PRINT_NUMBER(unsigned, 10, LOWER_CASE);
          break;

        case 'X':
          PRINT_NUMBER(unsigned, 16, UPPER_CASE);
          break;

        case 'x':
          PRINT_NUMBER(unsigned, 16, LOWER_CASE);
          break;

        case 'c':
          PRINT_CHAR((char) va_arg(list, int));
          break;

        case 'p':
          // pointers on x86_64 should be uint64_t
          // 0x11223344aabbccdd
          PRINT_STRING("0x");
          PRINT_NUMBER(long long unsigned, 16, UPPER_CASE);
          
          break;

        case 'h': // %h[h][dux]
          // h - half - int promoted from short
          // hh - half half - int promoted from char
          // TODO do you even have to do anything for these specifiers?
          format++;
          if (*format == 'h') {
            format++;
            switch (*format) {
              case 'i':
              case 'd':
                PRINT_NUMBER(int, 10, LOWER_CASE);
                break;
              case 'u':
                PRINT_NUMBER(unsigned, 10, LOWER_CASE);
                break;
              case 'x':
                PRINT_NUMBER(unsigned, 16, LOWER_CASE);
                break;
              case 'X':
                PRINT_NUMBER(unsigned, 16, UPPER_CASE);
                break;
            }
          } else {
            switch (*format) {
              case 'i':
              case 'd':
                PRINT_NUMBER(int, 10, LOWER_CASE);
                break;
              case 'u':
                PRINT_NUMBER(unsigned, 10, LOWER_CASE);
                break;
              case 'x':
                PRINT_NUMBER(unsigned, 16, LOWER_CASE);
                break;
              case 'X':
                PRINT_NUMBER(unsigned, 16, UPPER_CASE);
                break;
            }
          }
          break;

        case 'l': // %l[l][dux]
          // l - long - at least 32 bits
          // ll - long long - at least 64 bits
          format++;
          if (*format == 'l') {
            format++;
            switch (*format) {
              case 'i':
              case 'd':
                PRINT_NUMBER(long long int, 10, LOWER_CASE);
                break;
              case 'u':
                PRINT_NUMBER(long long unsigned, 10, LOWER_CASE);
                break;
              case 'x':
                PRINT_NUMBER(long long unsigned, 16, LOWER_CASE);
                break;
              case 'X':
                PRINT_NUMBER(long long unsigned, 16, UPPER_CASE);
                break;
            }
          } else {
            switch (*format) {
              case 'i':
              case 'd':
                PRINT_NUMBER(long int, 10, LOWER_CASE);
                break;
              case 'u':
                PRINT_NUMBER(long unsigned, 10, LOWER_CASE);
                break;
              case 'x':
                PRINT_NUMBER(long unsigned, 16, LOWER_CASE);
                break;
              case 'X':
                PRINT_NUMBER(long unsigned, 16, UPPER_CASE);
                break;
            }
          }
          break;

        case 'q': // %q[dux]
          // quad-word, 64 bits, same as "long long" or "ll"
          format++;
          switch (*format) {
            case 'i':
            case 'd':
              PRINT_NUMBER(long long int, 10, LOWER_CASE);
              break;
            case 'u':
              PRINT_NUMBER(long long unsigned, 10, LOWER_CASE);
              break;
            case 'x':
              PRINT_NUMBER(long long unsigned, 16, LOWER_CASE);
              break;
            case 'X':
              PRINT_NUMBER(long long unsigned, 16, UPPER_CASE);
              break;
          }
          break;

        case 's':
          PRINT_STRING(va_arg(list, char*));
          break;

        default:
          // unrecognized format character
          PRINT_CHAR('?');
          break;
      }
    } else {
      PRINT_CHAR(*format);
    }

    format++;
  }

  va_end(list);
  return 0;
}
