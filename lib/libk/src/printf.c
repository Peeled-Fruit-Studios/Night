#include <string.h>
#include <mm/pmm.h>
#include <drivers/scrn.h>

extern int p_online;

void printf(char* fmt, ...) {
  char* buf;
  if(p_online == 1) {
    buf = kmalloc(strlen(fmt) + 128);
  } else {
    char bufs[200];
    buf = &bufs;
  }
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  puts(buf);
  if(p_online == 1) {
    kfree(buf);
  }
  va_end(ap);
}