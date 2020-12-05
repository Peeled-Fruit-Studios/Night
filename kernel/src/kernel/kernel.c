/*  kernel/kernel.c - Kernel entry point.
 *  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
 *  See LICENSE for more information */

#include <kernel.h>
#include <libk/panic.h>

char term_buf[100];

void terminal() {
  puts("Night Kernel (0.5.3) (Official Build) (x86_32)\n");
  puts("Copyright (c) 2020 Peeled Fruit Studios. All Rights Reserved.\n");
  puts("\n$ ");
  tab_stop();
}

void main() {
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  init_video();
  puts("Night Kernel v0.5.3\n");
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
  puts("Initializing Paging\n");
  init_paging();


  __asm__ __volatile__("sti");
  lock_vga();
  sleep(4);
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  cls();
  unlock_vga();
  terminal();
  for (;;)
    ;
}
