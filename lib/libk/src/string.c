#include <memory.h>
#include <mm/pmm.h>
#include <string.h>

size_t strlen(const char *str) {
  size_t retval;
  for (retval = 0; *str != '\0'; str++) retval++;
  return retval;
}

char *ssplit(char to_split, char *src, char *rem) {
  for (int i = 0; i < strlen(src); i++) {
    if (src[i] == to_split) {
      rem = &src[i];
      char *trt = kmalloc(sizeof(char) * i);
      memcpy(src, trt, i);
      return trt;
    }
  }
  return NULL;
}

#define is_digit(c) ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s) {
  int i = 0;

  while (is_digit(**s)) i = i * 10 + *((*s)++) - '0';
  return i;
}

#define ZEROPAD 1
#define SIGN 2
#define PLUS 4
#define SPACE 8
#define LEFT 16
#define SPECIAL 32
#define SMALL 64

#define do_div(n, base)                                                    \
  ({                                                                       \
    int __res;                                                             \
    __asm__("divl %4" : "=a"(n), "=d"(__res) : "0"(n), "1"(0), "r"(base)); \
    __res;                                                                 \
  })

static char *number(char *str, int num, int base, int size, int precision,
                    int type) {
  char c, sign, tmp[36];
  const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i;

  if (type & SMALL) digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  if (type & LEFT) type &= ~ZEROPAD;
  if (base < 2 || base > 36) return 0;
  c = (type & ZEROPAD) ? '0' : ' ';
  if (type & SIGN && num < 0) {
    sign = '-';
    num = -num;
  } else
    sign = (type & PLUS) ? '+' : ((type & SPACE) ? ' ' : 0);
  if (sign) size--;
  if (type & SPECIAL) {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }
  i = 0;
  if (num == 0)
    tmp[i++] = '0';
  else
    while (num != 0) tmp[i++] = digits[do_div(num, base)];
  if (i > precision) precision = i;
  size -= precision;
  if (!(type & (ZEROPAD + LEFT)))
    while (size-- > 0) *str++ = ' ';
  if (sign) *str++ = sign;
  if (type & SPECIAL) {
    if (base == 8)
      *str++ = '0';
    else if (base == 16) {
      *str++ = '0';
      *str++ = digits[33];
    }
  }
  if (!(type & LEFT))
    while (size-- > 0) *str++ = c;
  while (i < precision--) *str++ = '0';
  while (i-- > 0) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';
  return str;
}

int vsprintf(char *buf, const char *fmt, va_list args) {
  int len;
  int i;
  char *str;
  char *s;
  int *ip;

  int flags;

  int field_width;
  int precision;
  int qualifier;

  for (str = buf; *fmt; ++fmt) {
    if (*fmt != '%') {
      *str++ = *fmt;
      continue;
    }

    flags = 0;
  repeat:
    ++fmt;
    switch (*fmt) {
      case '-':
        flags |= LEFT;
        goto repeat;
      case '+':
        flags |= PLUS;
        goto repeat;
      case ' ':
        flags |= SPACE;
        goto repeat;
      case '#':
        flags |= SPECIAL;
        goto repeat;
      case '0':
        flags |= ZEROPAD;
        goto repeat;
    }

    field_width = -1;
    if (is_digit(*fmt))
      field_width = skip_atoi(&fmt);
    else if (*fmt == '*') {
      field_width = va_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= LEFT;
      }
    }

    precision = -1;
    if (*fmt == '.') {
      ++fmt;
      if (is_digit(*fmt))
        precision = skip_atoi(&fmt);
      else if (*fmt == '*') {
        precision = va_arg(args, int);
      }
      if (precision < 0) precision = 0;
    }

    qualifier = -1;
    if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
      qualifier = *fmt;
      ++fmt;
    }

    switch (*fmt) {
      case 'c':
        if (!(flags & LEFT))
          while (--field_width > 0) *str++ = ' ';
        *str++ = (unsigned char)va_arg(args, int);
        while (--field_width > 0) *str++ = ' ';
        break;

      case 's':
        s = va_arg(args, char *);
        len = strlen(s);
        if (precision < 0)
          precision = len;
        else if (len > precision)
          len = precision;

        if (!(flags & LEFT))
          while (len < field_width--) *str++ = ' ';
        for (i = 0; i < len; ++i) *str++ = *s++;
        while (len < field_width--) *str++ = ' ';
        break;

      case 'o':
        str = number(str, va_arg(args, unsigned long), 8, field_width,
                     precision, flags);
        break;

      case 'p':
        if (field_width == -1) {
          field_width = 8;
          flags |= ZEROPAD;
        }
        str = number(str, (unsigned long)va_arg(args, void *), 16, field_width,
                     precision, flags);
        break;

      case 'x':
        flags |= SMALL;
      case 'X':
        str = number(str, va_arg(args, unsigned long), 16, field_width,
                     precision, flags);
        break;

      case 'd':
      case 'i':
        flags |= SIGN;
      case 'u':
        str = number(str, va_arg(args, unsigned long), 10, field_width,
                     precision, flags);
        break;

      case 'n':
        ip = va_arg(args, int *);
        *ip = (str - buf);
        break;

      default:
        if (*fmt != '%') *str++ = '%';
        if (*fmt)
          *str++ = *fmt;
        else
          --fmt;
        break;
    }
  }
  *str = '\0';
  return str - buf;
}

void sprintf(char *buf, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);
}

char *strdup(char *str) {
  volatile char *data = kmalloc(strlen(str));
  memcpy(data, str, strlen(str));
  return data;
}

char *strtok(char *str, const char *delim) {
  static char *left = NULL;
  if (str == NULL) str = left;
  if (str == NULL) return NULL;

  const char *p;
  bool flag;
  while (*str != 0) {
    p = delim;
    flag = false;
    while (*p != 0) {
      if (*p++ == *str) {
        flag = true;
        break;
      }
    }
    if (!flag) break;
    ++str;
  }
  char *s = str;
  while (*s != 0) {
    p = delim;
    flag = false;
    while (*p != 0) {
      if (*p++ == *s) {
        flag = true;
        *s = 0;
        break;
      }
    }
    ++s;
    if (flag) {
      left = s;
      return str;
    }
  }
  left = NULL;
  return str;
}