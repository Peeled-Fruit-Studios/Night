/* cpu/idt.c - IDT Configuration & Setup for Night kernel
*  Copyright (c) 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information */
#include <cpu/tables.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void _idt_load();

/* Use this function to set an entry in the IDT. Alot simpler
 *  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
                  unsigned char flags) {
  /* The interrupt routine's base address */
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;

  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install() {
  /* Sets the special IDT pointer up, just like in 'gdt.c' */
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = &idt;

  /* Clear out the entire IDT, initializing it to zeros */
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  /* Add any new ISRs to the IDT here using idt_set_gate */

  /* Points the processor's internal register to the new IDT */
  _idt_load();
}
