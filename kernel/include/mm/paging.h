#ifndef NIGHT_PAGING_H
#define NIGHT_PAGING_H

#include <libk/types.h>

typedef struct {
  u32 present : 1;
  u32 rw : 1;
  u32 user : 1;
  u32 accessed : 1;
  u32 dirty : 1;
  u32 used : 7;
  u32 frame : 20;
} m_page;


typedef struct {
  m_page pgs[1024];
} page_table;

typedef struct {
  page_table* tables[1024];
  u32 phys_tables[1024];
  u32 phys_adder;
} page_dir;

void init_paging();

void swap_page_dir(page_dir* pdir);

m_page* get_page(u32 address, int mode, page_dir* dir);

#endif // NIGHT_PAGING_H