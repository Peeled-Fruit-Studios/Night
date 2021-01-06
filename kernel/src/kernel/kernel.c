/*  kernel/kernel.c - Kernel entry point.
 *  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
 *  See LICENSE for more information */

#include <kernel.h>
#include <libk/panic.h>
#include <fs/fs.h>

extern fs_node* root;
extern size_t tar_read(size_t re, u8* bf, fs_node* kg);


void terminal() {
  printf("Night Kernel (%s) (%s) (x86_32)\n", KERNEL_VER, BUILD_ID);
  puts("Copyright (c) 2020 Peeled Fruit Studios. All Rights Reserved.\n");
  puts("\n$ ");
  tab_stop();
}


static void load_mods(multiboot* mboot) {
  // There is only one mod as of now.

  mod_t* tmod = mboot->mods_addr;
  printf("Loading module %s with sz of %d\n", tmod->string, tmod->mod_end - tmod->mod_start);
  fs_mount(init_tarfs((void*)tmod->mod_start));
}


void kmain(multiboot* mb) {
  set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  init_video();
  printf("Night Kernel %s\n", KERNEL_VER);
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
  puts("Loading Modules...\n");
  load_mods(mb);

  // Initialization is now complete, interupts can be enabled.
  __asm__ __volatile__("sti");
  sleep(2);
  cls();
  terminal();
  for (;;)
    ;
}
