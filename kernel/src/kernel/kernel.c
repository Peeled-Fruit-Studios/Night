/*  kernel/kernel.c - Kernel entry point.
 *  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
 *  See LICENSE for more information */

#include <kernel.h>
#include <libk/panic.h>
#include <fs/fs.h>

extern fs_node* root;

/*
char term_buf[100];

void terminal() {
  puts("Night Kernel (0.5.3) (Official Build) (x86_32)\n");
  puts("Copyright (c) 2020 Peeled Fruit Studios. All Rights Reserved.\n");
  puts("\n$ ");
  tab_stop();
}
*/

void fs_test() {
  puts("Opening file S:/kernel.conf\n");
  fs_node* fil = get_file("S:/kernel.conf");
  puts(fil->name);
  putch('\n');
  char* buf = kmalloc(fil->sz);
  fs_read(fil, fil->sz, buf);
  puts(buf);
}


void kmain(multiboot* mb) {
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
  puts("Initializing Memory\n");
  init_paging();
  init_pmm(mb);
  puts("Initializing Filesystem\n");
  init_fs();
#ifdef USE_INITRD
  u32 loc = *((u32*)mb->mods_addr);
  u32 end = *((u32*)mb->mods_addr + 4);
  u32 sz = end - loc;
  printf("Mod at addr 0x%x with size of %d\n", loc, sz);
  fs_mount(init_tarfs((void*)loc));
#endif
  __asm__ __volatile__("sti");
  lock_vga();
  sleep(4);
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  cls();
  unlock_vga();
  fs_test();
  /*
  terminal();
  */
  for (;;)
    ;
}
