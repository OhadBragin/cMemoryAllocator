#include "memalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>


#define ALIGNMENT (2 * sizeof(size_t))
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(size_t)(ALIGNMENT - 1))

#define HEAP_SIZE (1024 * 1024)

typedef struct block_header{
   size_t size; //allocated bytes
   size_t is_free; //is block free
   struct block_header *next; //next block -- linked list
   size_t magic; //magic to detect valid header. also serves as align to 16/32bytes
}block_header;


static _Alignas(size_t) uint8_t heap[HEAP_SIZE]; //1MB
static size_t heap_space = sizeof(heap);
static block_header *pLastBlock = NULL;
static block_header *pFirstBlock = NULL;
static void *heap_end = (void *)(heap + HEAP_SIZE);
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

static block_header* get_free_block(size_t nbytes) {
   //first search for a free block
   block_header *pCurrentBlock = pFirstBlock;

   while (pCurrentBlock != NULL) {
      if (pCurrentBlock->is_free == 1) {
         if (pCurrentBlock->size >= nbytes)
            return pCurrentBlock;
      }
      pCurrentBlock = pCurrentBlock->next;
   }
   //no blocks were found, create new one
   if (pLastBlock == NULL) { //first block
      init_header(&heap);
      pFirstBlock = (block_header*)heap;
      return pLastBlock;
   }
   //first, check if theres space
   size_t space_needed = ALIGN(sizeof(block_header) + nbytes);
   if (heap_space >= space_needed) {
      uint8_t *offset = (uint8_t*)pLastBlock;
      offset += sizeof(block_header) + pLastBlock->size;//get next header address
      if ((uintptr_t)offset >= (uintptr_t)heap_end) {
         return NULL;
      }
      init_header(offset);
      return pLastBlock;
   }
   //no blocks found, no space for new block
   return NULL;
}

void *my_malloc(size_t nbytes) {
   //get a free block, change it's is_free status,
   //assign data after the block.
   if (nbytes <= 0) {
      errno = EINVAL;
      return NULL;
   }
   //align bytes
   nbytes = ALIGN(nbytes);
   //get free block
   block_header *header = get_free_block(nbytes);
   if (header == NULL) {
      errno = ENOMEM;
      return NULL;
   }
   header->size = nbytes;
   header->is_free = 0;
   heap_space -= nbytes;
   header++;
   return header; //starting point of user access
}

void *my_calloc(size_t count, size_t size) {
   //check valid params
   if (count <= 0 || size <= 0) {
      errno = EINVAL;
      return NULL;
   }
   size_t space_needed = ALIGN(count * size);
   void *ptr = my_malloc(space_needed);
   //my_alloc returned null
   if (ptr == NULL) {
      return NULL;
   }
   //iterate with a for loop, setting each byte to zero.
   size_t *pSize = ptr;
   size_t const num_words = space_needed / sizeof(size_t);
   for (int i = 0; i < num_words; i++) {
      pSize[i] = 0;
   }
   return ptr;
}



void *is_valid_pointer(void *ptr) {
   //checks if the pointer was returned by a valid memory function
   //if not, returns NULL
   //if it is, returns a pointer to the start of the blocker header
   if (ptr == NULL) {
      return NULL;
   }
   //we need to get back to is_free(start of the header), which is supposed
   //to be 3 * size_t before the ptr
   size_t *pHeader = ptr;
   pHeader--; //first, we check if it is a valid block by checking the magic value
   if (*pHeader != 0xDEADBEEF) { //not a valid block
      return NULL;
   }
   pHeader -= 3; //size field, the head
   return pHeader;

}

void my_free(void* ptr) {
   //goal: set is_free to 0
   if (ptr == NULL) {
      return;
   }
   size_t *pIsFree = is_valid_pointer(ptr);
   if (pIsFree == NULL) {
      return;
   }
   heap_space +=  *pIsFree; //increase heap space
   pIsFree++; //is_free
   *pIsFree = 1; //set to true
}

void *my_realloc(void *ptr, size_t size) {
   void *res;
   size = ALIGN(size);
   if (ptr == NULL) {
      //if ptr is null, act like my_malloc() for size bytes
      res = my_malloc(size);
      return res;
   }
   else if (size <= 0) {
      //if size is 0 AND ptr is NOT NULL, allocate minimum sized object
      //and free ptr
      my_free(ptr);
      res = my_malloc(1);
      return res;
   }
   block_header *pHeader = is_valid_pointer(ptr);
   if (pHeader == NULL) {
      return NULL;
   }
   //first, we check if theres enough space for a reallocation
   //if there is, we just change the size field
   //if there isnt, we try to copy as much data to an new allocation,
   //and free the given ptr
   if (pHeader->size >= size) {
      pHeader->size = size;
      return ptr;
   }
   block_header *new_ptr = get_free_block(size);
   if (new_ptr == NULL) {
      errno = ENOMEM;
      exit(1);
   }
   new_ptr++; //user data entry point
   new_ptr = memcpy(new_ptr, ptr, size);
   my_free(ptr);
   return new_ptr;
}



