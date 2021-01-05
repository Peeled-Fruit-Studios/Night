#include <memory.h>


void* memcpy(void *dest, const void *src, size_t count) {
  const char *sp = (const char *)src;
  char *dp = (char *)dest;
  for (; count != 0; count--) *dp++ = *sp++;
  return dest;
}

int memcmp(u8* uno, u8* dos, int num) {
  for(int k = 0; k < num; k++) {
    if(uno[k] != dos[k]) {
      return 1;
    }
  }

  return 0;
}

void *memset(void *dest, char val, size_t count) {
  char *temp = (char *)dest;
  for (; count != 0; count--) *temp++ = val;
  return dest;
}

u16 *memsetw(u16 *dest, u16 val, size_t count) {
  u16 *temp = (u16 *)dest;
  for (; count != 0; count--) *temp++ = val;
  return dest;
}

u8 inb(u16 _port) {
  u8 rv;
  __asm__ __volatile__("inb %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

void outb(u16 _port, u8 _data) {
  __asm__ __volatile__("outb %1, %0" : : "dN"(_port), "a"(_data));
}

u32 divide_up(u32 n, u32 d) {
  if (n % d == 0) {
    return n / d;
  }

  return 1 + n / d;
}