#include <mm/paging.h>
#include <libk/string.h>

// A bitset of frames - used or free.
u32 *frames;
u32 nframes;

extern u32 padder;

page_directory_t *kernel_directory = 0;

page_directory_t *current_directory = 0;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

static void set_frame(u32 frame_addr) {
  u32 frame = frame_addr / 0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32 frame_addr) {
  u32 frame = frame_addr / 0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static u32 test_frame(u32 frame_addr) {
  u32 frame = frame_addr / 0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static u32 first_frame() {
  u32 i, j;
  for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
    if (frames[i] != 0xFFFFFFFF) {
      for (j = 0; j < 32; j++) {
        u32 toTest = 0x1 << j;
        if (!(frames[i] & toTest)) { return i * 4 * 8 + j; }
      }
    }
  }
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
  if (page->frame != 0) {
    return;  // Frame was already allocated, return straight away.
  } else {
    u32 idx = first_frame();  // idx is now the index of the first free frame.
    if (idx == (u32)-1) { kpanic("Out of Memory!"); }
    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = (is_writeable) ? 1 : 0;
    page->user = (is_kernel) ? 0 : 1;
    page->frame = idx;
  }
}

// Function to deallocate a frame.
void free_frame(page_t *page) {
  u32 frame;
  if (!(frame = page->frame)) {
    return;
  } else {
    clear_frame(frame);
    page->frame = 0x0;
  }
}

void switch_page_directory(page_directory_t *dir) {
  current_directory = dir;
  asm volatile("mov %0, %%cr3" ::"r"(&dir->tablesPhysical));
  u32 cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;  // Enable paging!
  asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

page_t *get_page(u32 address, int make, page_directory_t *dir) {
  // Turn the address into an index.
  address /= 0x1000;
  // Find the page table containing this address.
  u32 table_idx = address / 1024;
  if (dir->tables[table_idx]) {
    return &dir->tables[table_idx]->pages[address % 1024];
  } else if (make) {
    u32 tmp;
    dir->tables[table_idx] =
        (page_table_t *)kmalloc(sizeof(page_table_t), 1, &tmp);
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tablesPhysical[table_idx] = tmp | 0x7;  // PRESENT, RW, US.
    return &dir->tables[table_idx]->pages[address % 1024];
  } else {
    return 0;
  }
}

void init_paging() {
  u32 mem_end_page = 0x1000000;

  nframes = mem_end_page / 0x1000;
  frames = (u32 *)kmalloc(INDEX_FROM_BIT(nframes), 0, 0);
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  // Let's make a page directory.
  kernel_directory =
      (page_directory_t *)kmalloc(sizeof(page_directory_t), 1, 0);
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;
  int i = 0;
  while (i < padder) {
    // Kernel code is readable but not writeable from userspace.
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }
  switch_page_directory(kernel_directory);
}

void page_fault(regs rg) {
  u32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

  // The error code gives us details of what happened.
  int present = !(rg.err_code & 0x1);  // Page not present
  int rw = rg.err_code & 0x2;  // Write operation?
  int us = rg.err_code & 0x4;  // Processor was in user-mode?
  int reserved =
      rg.err_code & 0x8;  // Overwritten CPU-reserved bits of page entry?
  int id = rg.err_code & 0x10;  // Caused by an instruction fetch?

  // Output an error message.
  puts("Page fault! ( ");
  if (present) { puts("present "); }
  if (rw) { puts("read-only "); }
  if (us) { puts("user-mode "); }
  if (reserved) { puts("reserved "); }
  puts(")\n");
  if(faulting_address != 0xA0000000) {
    puts("Something weird is going on\n");
  } else {
    puts("good to go\n");
  }
  puts("\n");
}