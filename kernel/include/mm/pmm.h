#ifndef NIGHT_PMM_H
#define NIGHT_PMM_H

#include <libk/types.h>
#include <multiboot.h>
#include <liballoc/alloc.h>


void init_pmm(multiboot* boot);
u32 pmm_used_memory();
u32 pmm_total_memory();
void pmm_init_region(addr_t addr, u32 size);
void pmm_deinit_region(addr_t addr, u32 size);
addr_t pmm_alloc_page();
addr_t pmm_alloc_aligned_large_page();
addr_t pmm_alloc_pages(u32 num);
void pmm_free_page(addr_t addr);
void pmm_free_pages(addr_t addr, u32 num);
u32 pmm_get_kernel_end();

extern u32* mem_map;

#define PMM_BLOCK_SIZE 4096

#endif // NIGHT_PMM_H