#ifndef NIGHT_PAGING_H
#define NIGHT_PAGING_H

#include <libk/types.h>
#include <libk/memory.h>
#include <libk/panic.h>

#include <cpu/regs.h>


typedef struct page {
  u32 present : 1;  
  u32 rw : 1; 
  u32 user : 1;  
  u32 accessed : 1;  
  u32 dirty : 1;  
  u32 unused : 7;  
  u32 frame : 20;  
} page_t;

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  page_table_t *tables[1024];
  u32 tablesPhysical[1024];
  u32 physicalAddr;
} page_directory_t;


void init_paging();
void switch_page_directory(page_directory_t *new);
page_t *get_page(u32 address, int make, page_directory_t *dir);
void page_fault(regs rg);

#endif  // NIGHT_PAGING_H