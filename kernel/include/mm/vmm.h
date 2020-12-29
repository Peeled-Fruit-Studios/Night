#ifndef NIGHT_VMM_H
#define NIGHT_VMM_H

#include <libk/types.h>
#include <mm/list.h>

#define	KERN_PDIR			0x00001000
#define	KERN_STACK			0x0009FFF0
#define	KERN_BASE			0x00100000
#define KERN_PG_HEAP		0x00800000
#define KERN_PG_HEAP_LIM	0x10000000
#define KERN_HEAP			0x10000000
#define KERN_HEAP_LIM		0x40000000

#define	USER_OFFSET 		0x40000000
#define	USER_STACK 			0xE0000000
	
#define KERN_PG_1			0x400000
#define KERN_PG_1_LIM 		0x800000

#define	VADDR_PD_OFFSET(addr)	((addr) & 0xFFC00000) >> 22
#define	VADDR_PT_OFFSET(addr)	((addr) & 0x003FF000) >> 12
#define	VADDR_PG_OFFSET(addr)	(addr) & 0x00000FFF
#define PAGE(addr)		(addr) >> 12

#define	PAGING_FLAG 		0x80000000	/* CR0 - bit 31 */
#define PSE_FLAG			0x00000010	/* CR4 - bit 4  */

#define PG_PRESENT			0x00000001	/* page directory / table */
#define PG_WRITE			0x00000002
#define PG_USER				0x00000004
#define PG_4MB				0x00000080

#define	PAGESIZE 			4096
#define	RAM_MAXSIZE			0x100000000
#define	RAM_MAXPAGE			0x100000

typedef struct page {
  char *v_addr;
  char *p_addr;
  list_head list;
} page;

typedef struct page_directory{
  page *base;
  list_head pt;
} page_directory;

struct vm_area {
  char *vm_start;
  char *vm_end; 
  list_head list;
};

typedef struct vm_area vm_area;

typedef page_directory proc_memory;

extern char *kern_heap;

extern list_head kern_free_vm;

extern u32 *pd0;
extern u8 mem_bitmap[];

extern u32 kmalloc_used;

#define set_page_frame_used(page) \
  mem_bitmap[((u32)page) / 8] |= (1 << (((u32)page) % 8))
#define release_page_frame(p_addr)            \
  mem_bitmap[((u32)p_addr / PAGESIZE) / 8] &= \
      ~(1 << (((u32)p_addr / PAGESIZE) % 8))

char *get_page_frame(void);

struct page *get_page_from_heap(void);
int release_page_from_heap(char *v_addr);

void init_memory(u32 high_mem);

struct page_directory *pd_create(void);
int pd_destroy(struct page_directory *pd);
struct page_directory *pd_copy(struct page_directory *pdfather);

int pd0_add_page(char *v_addr, char *p_addr, int flags);

int pd_add_page(char *v_addr, char *p_addr, int flags, struct page_directory *pd);
int pd_remove_page(char *v_addr);

char *get_p_addr(char *v_addr);

void vmm_map(char* phys, char* virt);

#define KMALLOC_MINSIZE 16

struct kmalloc_header {
  unsigned long size : 31; 
  unsigned long used : 1;
} __attribute__((packed));

#endif