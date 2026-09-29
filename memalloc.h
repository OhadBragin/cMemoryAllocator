#ifndef CMEMORYALLOCATOR_MEMALLOC_H
#define CMEMORYALLOCATOR_MEMALLOC_H

#include <stddef.h>

void *my_malloc(size_t nbytes);
void my_free(void *ptr);
void *my_calloc(size_t count, size_t size);
void *my_realloc(void *ptr, size_t size);

#endif //CMEMORYALLOCATOR_MEMALLOC_H
