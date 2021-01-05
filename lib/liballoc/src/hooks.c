#include <alloc.h>
#include <mm/pmm.h>

// No work needed because smp isn't added yet.
int liballoc_lock() { return 0; }
int liballoc_unlock() { return 0; }

void* liballoc_alloc(size_t num) {
  return pmm_alloc_pages((u32)num);
}

int liballoc_free(void* addr, size_t num) {
  pmm_free_pages(addr, (u32)num);
  return 0;
}