/*******************************************
*       gdt.c - GDT Init and Setup         *
*  Copyright (c) 2020 PFS and Contributors *
*          --------------------            *
*     See LICENSE for more information     *
*******************************************/

#include <cpu/tables.h>
#include <libk/types.h>

struct gdt_entry gdt[5];
struct gdt_ptr gp;

extern void _gdt_flush();


void gdt_set_gate(int num, size_t base, size_t limit, u8 access, u8 gran) {
  
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}

void gdt_install() {
  
  gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gp.base = &gdt;

  
  gdt_set_gate(0, 0, 0, 0, 0); // NULL Segment (Required)
  
  // Kernel Code Segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  // Kernel Data Segment
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  // User Code Segment
  gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  // User Data Segment
  gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

  
  _gdt_flush(); // Extern asm function defined in start.s
}
