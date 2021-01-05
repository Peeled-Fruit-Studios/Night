#include <multiboot.h>
#include <mm/paging.h>  
#include <mm/pmm.h>

#include <libk/print.h>

#define NTHBIT(n) ((u32)1 << n)
#define PHYS_TO_VIRT(addr) ((addr) + KERNEL_BASE_VIRT)
#define VIRT_TO_PHYS(addr) ((addr) -KERNEL_BASE_VIRT)

static u32 bitmap[1024 * 1024 / 32];
static u32 mem_size;
static u32 used_blocks;
static u32 max_blocks;
static addr_t kernel_end;

#define KERNEL_END end

extern u32 end;
u32 KERNEL_END_PHYS = 0x0100000;

void mmap_set(u32 bit);
void mmap_unset(u32 bit);
u32 mmap_test(u32 bit);
u32 mmap_find_free();
u32 mmap_find_free_frame(u32 num);

void init_pmm(multiboot* boot) {
  
  mod_t* modules = (mod_t*)boot->mods_addr;

  for (u32 i = 0; i < boot->mods_count; i++) {
    mod_t mod = modules[i];
    kernel_end = mod.mod_end;
  }

  
  
  if (kernel_end < (addr_t)&KERNEL_END_PHYS) {
    kernel_end = (addr_t)&KERNEL_END_PHYS;
  }

  if ((u32)&KERNEL_END > KERNEL_END_MAP) {
    kpanic("The kernel is too large for its initial mapping");
  }

  
  mem_size = (u32)boot->mem_lower + (u32)boot->mem_upper;
  max_blocks = (mem_size * 1024) / PMM_BLOCK_SIZE;  
  used_blocks = max_blocks;
  memset(bitmap, 0xFF, max_blocks / 8);  

  
  size_t available = 0;
  size_t unavailable = 0;

  mmap_t* mmap = (mmap_t*)PHYS_TO_VIRT(boot->mmap_addr);

  while ((addr_t)mmap <
         (PHYS_TO_VIRT((u32)boot->mmap_addr) + (u32)boot->mmap_length)) {
    if (!mmap->length) { continue; }

    if (mmap->type == 1) {
      pmm_init_region((addr_t)mmap->addr, mmap->length);
      available += mmap->length;
    } else {
      unavailable += mmap->length;
    }

    
    mmap = (mmap_t*)((addr_t)mmap + mmap->size + sizeof(addr_t));
  }

  mem_size = available;

  
  pmm_deinit_region(0, kernel_end + max_blocks / 8);

  printf("memory stats: available: %d MiB", available >> 20);
  printf("unavailable: %d KiB", unavailable >> 10);
  printf("taken by modules: %d MiB",
         (kernel_end - (addr_t)&KERNEL_END_PHYS) >> 20);
}


u32 pmm_used_memory() { return used_blocks * PMM_BLOCK_SIZE; }


u32 pmm_total_memory() { return mem_size; }


void pmm_init_region(addr_t addr, u32 size) {
  
  u32 base_block = addr / PMM_BLOCK_SIZE;

  
  u32 num = divide_up(size, PMM_BLOCK_SIZE);

  while (num-- > 0) { mmap_unset(base_block++); }

  
  mmap_set(0);
}


void pmm_deinit_region(addr_t addr, u32 size) {
  u32 base_block = addr / PMM_BLOCK_SIZE;
  u32 num = divide_up(size, PMM_BLOCK_SIZE);

  while (num-- > 0) { mmap_set(base_block++); }
}


addr_t pmm_alloc_page() {
  if (max_blocks - used_blocks <= 0) {
    kpanic("kernel is out of physical memory!");
  }

  u32 block = mmap_find_free();

  if (!block) { return 0; }

  mmap_set(block);

  return (addr_t)(block * PMM_BLOCK_SIZE);
}


addr_t pmm_alloc_aligned_large_page() {  
  if (max_blocks - used_blocks < 2 * 1024) {  
    return 0;
  }

  u32 free_block = mmap_find_free_frame(2 * 1024);

  if (!free_block) { return 0; }

  u32 aligned_block = ((free_block / 1024) + 1) * 1024;

  for (int i = 0; i < 1024; i++) { mmap_set(aligned_block + i); }

  return (addr_t)(aligned_block * PMM_BLOCK_SIZE);
}

addr_t pmm_alloc_pages(u32 num) {
  if (max_blocks - used_blocks < num) { return 0; }

  u32 first_block = mmap_find_free_frame(num);

  if (!first_block) { return 0; }

  for (u32 i = 0; i < num; i++) { mmap_set(first_block + i); }

  return (addr_t)(first_block * PMM_BLOCK_SIZE);
}

void pmm_free_page(addr_t addr) {
  u32 block = addr / PMM_BLOCK_SIZE;
  mmap_unset(block);
}

void pmm_free_pages(addr_t addr, u32 num) {
  u32 first_block = addr / PMM_BLOCK_SIZE;

  for (u32 i = 0; i < num; i++) { mmap_unset(first_block + i); }
}

void mmap_set(u32 bit) {
  bitmap[bit / 32] |= NTHBIT(bit % 32);
  used_blocks++;
}

void mmap_unset(u32 bit) {
  bitmap[bit / 32] &= ~NTHBIT(bit % 32);
  used_blocks--;
}

u32 mmap_test(u32 bit) { return bitmap[bit / 32] & NTHBIT(bit % 32); }


u32 mmap_find_free() {
  for (u32 i = 0; i < max_blocks / 32; i++) {
    if (bitmap[i] != 0xFFFFFFFF) {
      for (u32 j = 0; j < 32; j++) {
        if (!(bitmap[i] & NTHBIT(j))) { return i * 32 + j; }
      }
    }
  }

  return 0;
}


u32 mmap_find_free_frame(u32 frame_size) {
  u32 first = 0;
  u32 count = 0;

  for (u32 i = 0; i < max_blocks / 32; i++) {
    if (bitmap[i] != 0xFFFFFFFF) {
      for (u32 j = 0; j < 32; j++) {
        if (!(bitmap[i] & NTHBIT(j))) {
          if (!first) { first = i * 32 + j; }

          count++;
        } else {
          first = 0;
          count = 0;
        }

        if (count == frame_size) { return first; }
      }
    } else {
      first = 0;
      count = 0;
    }
  }

  return 0;
}


u32 pmm_get_kernel_end() {
  return (u32)kernel_end + max_blocks / 8;
}