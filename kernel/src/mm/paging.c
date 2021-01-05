#include <mm/paging.h>
#include <mm/pmm.h>
#include <libk/print.h>

#define DIRECTORY_INDEX(x) ((x) >> 22)
#define TABLE_INDEX(x) (((x) >> 12) & 0x3FF)

static directory_entry_t* current_page_directory;

directory_entry_t kernel_directory[1024];

// Used by kernel to determine if paging was enabled
int p_online = 0;


void init_paging() {
  if(p_online == 1) {
    return;
  }
  u32 dir_phys = VIRT_TO_PHYS((addr_t)&kernel_directory);
  kernel_directory[1023] = dir_phys | PAGE_PRESENT | PAGE_RW;
  paging_invalidate_page(0xFFC00000);

  u32 to_map = divide_up(pmm_get_kernel_end(), 0x1000);
  kernel_directory[0] = 0;
  paging_map_pages(0x00000000, 0x00000000, to_map, PAGE_RW);
  paging_invalidate_page(0x00000000);
  current_page_directory = kernel_directory;
  p_online = 1;
}

addr_t paging_get_kernel_directory() {
  return VIRT_TO_PHYS((addr_t)&kernel_directory);
}

page_t* paging_get_page(u32 virt, bool create, u32 flags) {
  if (virt % 0x1000) {
    printf("Paging_get_page: unaligned address %p", virt);
  }

  u32 dir_index = DIRECTORY_INDEX(virt);
  u32 table_index = TABLE_INDEX(virt);

  directory_entry_t* dir = (directory_entry_t*)0xFFFFF000;
  page_t* table = (page_t*)(0xFFC00000 + (dir_index << 12));

  if (!(dir[dir_index] & PAGE_PRESENT) && create) {
    page_t* new_table = (page_t*)pmm_alloc_page();
    dir[dir_index] =
        (u32)new_table | PAGE_PRESENT | PAGE_RW | (flags & PAGE_FLAGS);
    memset((addr_t)table, 0, 4096);
  }

  if (dir[dir_index] & PAGE_PRESENT) { return &table[table_index]; }

  return NULL;
}

void paging_map_page(addr_t virt, addr_t phys, u32 flags) {
  page_t* page = paging_get_page(virt, true, flags);

  if (*page & PAGE_PRESENT) {
    printf("tried to map an already mapped virtual address 0x%X to 0x%X", virt,
            phys);
    printf("previous mapping: 0x%X to 0x%X", virt, *page & PAGE_FRAME);
    kpanic("Mapping Error");
  }

  *page = phys | PAGE_PRESENT | (flags & PAGE_FLAGS);
  paging_invalidate_page(virt);
}

void paging_unmap_page(addr_t virt) {
  page_t* page = paging_get_page(virt, false, 0);

  if (page) {
    pmm_free_page(*page & PAGE_FRAME);
    *page = 0;
  }
}

void paging_map_pages(addr_t virt, addr_t phys, u32 num, u32 flags) {
  for (u32 i = 0; i < num; i++) {
    paging_map_page(virt, phys, flags);
    phys += 0x1000;
    virt += 0x1000;
  }
}

void paging_unmap_pages(addr_t virt, u32 num) {
  for (u32 i = 0; i < num; i++) {
    paging_unmap_page(virt);
    virt += 0x1000;
  }
}

void paging_switch_directory(addr_t dir_phys) {
  asm volatile("mov %0, %%cr3\n" ::"r"(dir_phys));
}

void paging_invalidate_cache() {
  asm("mov %cr3, %eax\n"
      "mov %eax, %cr3\n");
}

void paging_invalidate_page(addr_t virt) {
  asm volatile("invlpg (%0)" ::"b"(virt) : "memory");
}

void pg_handle(regs* r) {
  u32 faultAddr = 0;
  int error_code = r->err_code;

  __asm__ __volatile__(
      "mov %%cr2, %%eax	\n"
      "mov %%eax, %0			\n"
      : "=r"(faultAddr));

  printf("\nPage Fault at 0x%x", faultAddr);
  printf("\nReason: %s, %s, %s%s%s\n",
         error_code & 0b1 ? "protection violation" : "non-present page",
         error_code & 0b10 ? "write" : "read",
         error_code & 0b100 ? "user mode" : "supervisor mode",
         error_code & 0b1000 ? ", reserved" : "",
         error_code & 0b10000 ? ", instruction fetch" : "");

  for (;;)
    ;
}

addr_t paging_alloc_pages(addr_t virt, u32 size) {
  for (u32 i = 0; i < size; i++) {
    addr_t page = pmm_alloc_page();

    if (!page) { return NULL; }

    page_t* p = paging_get_page(virt + i * 0x1000, true, PAGE_RW | PAGE_USER);
    *p = page | PAGE_PRESENT | PAGE_RW | PAGE_USER;
  }

  return (addr_t)virt;
}

addr_t paging_virt_to_phys(addr_t virt) {
  page_t* p = paging_get_page(virt & PAGE_FRAME, false, 0);

  if (!p) { return 0; }

  return (((addr_t)*p) & PAGE_FRAME) + (virt & 0xFFF);
}