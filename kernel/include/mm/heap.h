#ifndef NIGHT_HEAP_H
#define NIGHT_HEAP_H

#include <libk/types.h>

typedef struct {
  struct heap_block* next;
  u32 size;
  u32 used;
  u32 bsize;
  u32 lfb;
} heap_block

void init_heap();

void *kmalloc(size_t size);

void free(void* ptr);

#endif // NIGHT_HEAP_H