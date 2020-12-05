#include <mm/heap.h>

extern u32 end;
u32 padder = (u32)&end;

u32 kmalloc(size_t size, int align, u32 *addr) {
  if(align == 1 && (padder & 0xFFFFF000)) {
    padder &= 0xFFFFF000;
    padder += 0x1000;
  }
  if(addr) {
    *addr = padder;
  }

  u32 tmp = padder;
  padder += size;
  return tmp;
}

