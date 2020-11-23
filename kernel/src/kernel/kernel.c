/* kernel/kernel.c - Kernel entry point.
 *  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
 *  See LICENSE for more information */

#include <kernel.h>

void main() {
  set_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
  init_video();
  puts("Night Kernel v0.5.1\nInitializing...\n");
  puts("Setting up Global Descriptor Tables...\n");
  gdt_install();
  puts("Setting up Interrupts & Drivers...\n");
  idt_install();
  isr_install();
  irq_install();
  puts("Setting up System Clock...\n");
  timer_install();
  puts("Setting up Keyboard...\n");
  keyboard_install();
  puts("Complete!\n");

  __asm__ __volatile__("sti");

  sleep(4);
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  cls();

  puts("$ ");

  for (;;)
    ;
}
