#include <string.h>
#include <mm/alloc.h>
#include <drivers/scrn.h>

void printf(char* fmt, ...) {
  char* buf = kmalloc(strlen(fmt) + 128);
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  puts(buf);
  kfree(buf);
  va_end(ap);
}