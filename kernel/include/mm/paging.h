#ifndef NIGHT_PAGING_H
#define NIGHT_PAGING_H

#include <libk/types.h>
#include <cpu/regs.h>

typedef u32 directory_entry_t;
typedef u32 page_t;

#define KERNEL_BASE_VIRT 0x0

/* The kernel is mapped in higher half using a 4 MiB page.
 * This is where it ends.
 */
#define KERNEL_END_MAP 0x400000

#define KERNEL_HEAP_BEGIN KERNEL_END_MAP
#define KERNEL_HEAP_SIZE 0x1E00000

#define PAGE_PRESENT 1
#define PAGE_RW      2
#define PAGE_USER    4
#define PAGE_LARGE   128

#define PAGE_FRAME   0xFFFFF000
#define PAGE_FLAGS   0x00000FFF

#define PHYS_TO_VIRT(addr) ((addr) + KERNEL_BASE_VIRT)
#define VIRT_TO_PHYS(addr) ((addr) -KERNEL_BASE_VIRT)

void init_paging();
addr_t paging_get_kernel_directory();
page_t* paging_get_page(addr_t virt, bool create, u32 flags);
void paging_map_page(addr_t virt, addr_t phys, u32 flags);
void paging_unmap_page(addr_t virt);
void paging_map_pages(addr_t phys, addr_t virt, u32 num, u32 flags);
void paging_unmap_pages(addr_t virt, u32 num);
void paging_switch_directory(addr_t dir_phys);
void paging_invalidate_cache();
void paging_invalidate_page(addr_t virt);
addr_t paging_alloc_pages(u32 virt, u32 num);
void paging_free_pages(addr_t virt, u32 num);
addr_t paging_virt_to_phys(addr_t virt);

#endif // NIGHT_PAGING_H