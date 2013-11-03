/* mem.c : memory manager
 */

#include <i386.h>
#include <xeroskernel.h>
#include <xeroslib.h>

typedef struct {
  unsigned long size;
  struct MemoryHeader *prev;
  struct MemoryHeader *next;
  char *sanity_check;
  unsigned char data_start[0];
} MemoryHeader;

/* Pointer to first memory in the free mem linked list */
MemoryHeader *memory_start;

/* Initializes values of memory_start and sets up HOLE as an allocated block */
extern void kmeminit( void ) {
  memory_start = freemem;
  MemoryHeader *after_hole = HOLEEND + 1;

  memory_start->prev = NULL;
  memory_start->next = after_hole;
  memory_start->size = HOLESTART - freemem - sizeof(MemoryHeader);
  
  after_hole->prev = memory_start;
  after_hole->next = NULL;
  after_hole->size = maxaddr - HOLEEND - sizeof(MemoryHeader);
}


/* Allocates memory block of required size from the free memory list.
 * First large enough block is chosen, and if extra space remains in the block
 * another free memory block referring to the left over space is created
 */
extern void *kmalloc( int size ) {
  // Set size to nearest paragraph
  int amnt = size/16 + ((size%16) ? 1 : 0);
  size = amnt*16;
  
  // Iterate over free memory list to find a suitable block
  if (memory_start == NULL) return NULL; // Out of Memory!

  MemoryHeader *mem = memory_start;
  while (size > mem->size) {
    if (mem->next == NULL) { 
      return NULL;  // No free block large enough!
    }
    else mem = mem->next;
  }
   
  // Create free memory header for remaining memory if sufficient
  if ((mem->size - size) > sizeof(MemoryHeader)) {
    MemoryHeader *remaining = mem->data_start + size + 1;
    remaining->size = mem->size - size - sizeof(MemoryHeader);
    
    remaining->next = mem->next;
    remaining->prev = mem->prev;
    
    if (mem->prev != NULL) ((MemoryHeader *)(mem->prev))->next = remaining;
    if (mem->next != NULL) ((MemoryHeader *)(mem->next))->prev = remaining;

    if (memory_start == mem) memory_start = remaining;
    mem->next = remaining;
  }
  else {
    if (mem->prev != NULL) ((MemoryHeader *)(mem->prev))->next = mem->next;
    if (mem->next != NULL) ((MemoryHeader *)(mem->next))->prev = mem->prev;
    
    if (memory_start == mem) memory_start = mem->next;
  }
  mem->sanity_check = size;
  mem->size = size;
  
  return mem->data_start;
}

/* Frees memory block at specified address and adds it back to the free memory list */
extern void kfree( void *ptr ) {
  MemoryHeader *mem = ptr - sizeof(MemoryHeader);
  if (memory_start == NULL) return;
  
  mem->next = memory_start;
  memory_start->prev = mem;
  memory_start = mem;

  // Joining adjacent free memory blocks
  MemoryHeader * iter = memory_start->next;
  while (iter != NULL) {
    // Check if space right after mem is also free
    if (mem->data_start+mem->size == iter) {    
      mem->next = iter->next;
      if (iter->next != NULL) ((MemoryHeader *) iter->next)->prev = mem;
      mem->size += sizeof(MemoryHeader) + iter->size;
    }
    iter = iter->next;
  }
  iter = memory_start->next;
  while (iter != NULL) {
    // Check if there is a free block right before mem
    if (iter->data_start+iter->size+1 == mem) {  
       iter->next = mem->next;
       if (mem->next != NULL) ((MemoryHeader *) mem->next)->prev = iter;
       iter->size = iter->size + sizeof(MemoryHeader) + mem->size;
    }
    iter = iter->next;
  }
}

/* Debug method for testing */
void memory_debug() {
  kprintf("\nMemory Linked list");
  kprintf("\n-----------------------------");
  
  int i = 0;
  MemoryHeader *mheader = memory_start;
  
  do {
    kprintf("\nAt position %x with address %ld ", i, mheader);
    kprintf("\n\tPrev = %ld, Next = %ld ", mheader->prev, mheader->next);
    kprintf("\n\tStarts at %ld with size %ld \n", mheader->data_start, mheader->size);

    mheader = mheader->next;
    i++;
  } while (mheader != NULL);
    
}
