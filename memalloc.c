#include "memalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define ALIGNMENT (2 * sizeof(size_t))
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(size_t)(ALIGNMENT - 1))

#define HEAP_SIZE (1024 * 1024)

typedef struct block_header{
   size_t size; //allocated bytes
   size_t is_free; //is block free
   struct block_header *next; //next block -- linked list
   size_t magic; //magic to detect valid header. also serves as align to 16/32bytes
}block_header;

_Alignas(size_t) uint8_t heap[HEAP_SIZE];
static size_t heap_space = sizeof(heap);
static uint8_t* pHeapHead = &heap[0];
static block_header* pLastBlock = NULL;

static void init_header(void *addr) {
    if (pLastBlock != NULL) { //not the first block
       pLastBlock->next = addr;
    }
   block_header *p = addr;
   p->size = 0;
   p->is_free = 1;
   p->next = NULL;
   p->magic = 0xDEADBEEF; //magic. not secure but yea
   pLastBlock = p;
   heap_space -= sizeof(block_header);
}

void init_heap() {
   init_header(&heap);
}



static block_header* get_free_block() {
   //first search for a free block
   block_header *pCurrentBlock = (block_header*)pHeapHead;
   //check first(in case theres only one head)
   if (pCurrentBlock->is_free) {
      return pCurrentBlock;
   }
   while (pCurrentBlock->next != NULL) {
      if (pCurrentBlock->is_free == 1) {
         return pCurrentBlock;
      }
      pCurrentBlock = pCurrentBlock->next;
   }
   //no blocks were found, create new one
   //first, check if theres space
   if (heap_space - sizeof(block_header) > 0) {
      uint8_t *offset = (uint8_t*)pCurrentBlock;
      offset += sizeof(block_header) + pCurrentBlock->size + 1;//get next header address
      init_header(offset);
      return pCurrentBlock->next;
   }
   //no blocks found, no space for new block
   return NULL;
}

void* my_malloc(size_t nbytes) {
   //get a free block, change it's is_free status,
   //assign data after the block.
   if (nbytes <= 0) {
      perror("\nmy_malloc(): nbytes must be more than 0!");
      return NULL;
   }
   //align bytes
   nbytes = ALIGN(nbytes);
   //check if theres space
   if (heap_space - nbytes < 0) {
      perror("\nmy_malloc():\n");
      perror(ErrorCode_to_text(ERR_OUT_OF_MEMEORY));
   }
   //get free block
   block_header *header = get_free_block();
   if (header == NULL) {
      perror(ErrorCode_to_text(ERR_OUT_OF_MEMEORY));
      exit(ERR_OUT_OF_MEMEORY);
   }
   header->size = nbytes;
   header->is_free = 0;
   heap_space -= nbytes;
   header++;
   return header; //starting point of user access
}

void my_free(void* ptr) {
   //goal: set is_free to 0
   if (ptr == NULL) {
      perror("my_free(): Pointer is empty!\n");
   }
   //we need to get back to is_free, which is supposed
   //to be 3 * size_t before the addr
   size_t *pIsFree = (size_t*)ptr;
   pIsFree--; //first, we check if it is a valid block by checking the magic value
   if (*pIsFree != 0xDEADBEEF) {
      perror("\nmy_free():\n");
      perror(ErrorCode_to_text(ERR_INVALID_ADDRESS));
      return;
   }
   pIsFree -= 2; //is_free
   *pIsFree = 1; //set to true
   pIsFree--; //get to size to increase heap space
   heap_space +=  *pIsFree;
}

void debug_print_heap() {
   printf("%lu, %lu", sizeof(heap), heap_space);
   size_t n = sizeof(heap) - heap_space;
   for (int i = 0; i < n; i++) {
      printf("%02x\n", heap[i]);
   }
}



