#ifndef NIGHT_LIBALLOC_H
#define NIGHT_LIBALLOC_H

#include <libk/types.h>

#define PREFIX(func)		k ## func

extern int liballoc_lock();
extern int liballoc_unlock();
extern void* liballoc_alloc(size_t);
extern int liballoc_free(void*,size_t);

extern void *PREFIX(malloc)(size_t);	
extern void *PREFIX(realloc)(void *, size_t);
extern void *PREFIX(calloc)(size_t, size_t);
extern void  PREFIX(free)(void *);


#endif