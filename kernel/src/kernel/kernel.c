/* kernel/kernel.c - Kernel entry point.
*  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information */

#include <system.h>

void main()
{
    int i;

    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    keyboard_install();

    __asm__ __volatile__ ("sti");

    puts("Night Kernel v0.5.0\n$ ");

    for (;;);
}
