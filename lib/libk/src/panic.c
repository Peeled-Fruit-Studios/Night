#include <panic.h>
#include <string.h>
#include <types.h>

// I hate to do it, but if I don't, the terminal will be filled with junk

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wpointer-sign"

static void print_buf() {
  for(int i = 0; i < 80; i++) {
    putch("-");
  }
}
/* I know the code in this function dosen't look pretty, but it's the best that I can muster for now. */

void kpanic(char* message) {
  cls();
  print_buf();
  puts("| Unfortunately, An Unrecoverable Error has occured :-(");
  putch("|");
  puts("| Reason: ");
  set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
  puts(message);
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  size_t strize = strlen(message) + 9;
  for(size_t i = strize; i < 79; i++) {
    putch(" ");
  }
  putch("|");
  print_buf();
}
