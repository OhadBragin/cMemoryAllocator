#include "memalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_SIZE 1024 * 1024

typedef struct block_header{
   size_t size; //allocated bytes
   size_t is_free; //is block free
   struct block_header *next; //next block -- linked list
   size_t alignment_pad; //align to 16/32bytes
}block_header;

_Alignas(size_t) uint8_t heap[HEAP_SIZE];
uint8_t* pHeapHead = &heap[0];
block_header* pLastBlock = NULL;

block_header* init_header(void *addr) {
    if (pLastBlock != NULL) { //not the first block
       pLastBlock->next = addr;
    }
   block_header *p = addr;
   p->size = 0;
   p->is_free = 1;
   p->next = NULL;
   p->alignment_pad = 0;
   pLastBlock = p;
}

void init_heap() {
   init_header(&heap[0]);
}



block_header* find_free_block() {
   //first search for a free block
   uint8_t *pCurrentBlock = pHeapHead;
   pCurrentBlock += 16; //next is 16 bytes from the start of the header
   while ((block_header*)pCurrentBlock->next)

}

void* my_alloc(size_t nbytes) {
   //TODO:
   //get a free block, change it's is_free status,
   //assign data after the block.
}

