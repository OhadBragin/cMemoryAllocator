
#ifndef CMEMORYALLOCATOR_MEMALLOC_H
#define CMEMORYALLOCATOR_MEMALLOC_H
#include <stddef.h>

void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_calloc(size_t num, size_t nsize);
void* my_realloc(void* ptr, size_t size);

void print_mem_blocks(void);

#endif //CMEMORYALLOCATOR_MEMALLOC_H
