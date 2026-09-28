
#ifndef CMEMORYALLOCATOR_MEMALLOC_H
#define CMEMORYALLOCATOR_MEMALLOC_H
#include <stddef.h>

typedef enum {
    ERR_OK = 0,
    ERR_OUT_OF_MEMEORY,
    ERR_INVALID_ADDRESS
}ErrorCode;

const char *ErrorCode_to_text(ErrorCode err);
void init_heap();
void debug_print_heap();
void* my_malloc(size_t nbytes);
void my_free(void* ptr);
void *my_calloc(size_t count, size_t size);
void* my_realloc(void* ptr, size_t size);

void print_mem_blocks(void);

#endif //CMEMORYALLOCATOR_MEMALLOC_H
