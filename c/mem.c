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

#define MEMORY_HEADER_SIZE sizeof(MemoryHeader)
MemoryHeader *memory_start;

extern void kmeminit( void ) {
  memory_start = freemem;
  MemoryHeader *after_hole = HOLEEND + 1;

  memory_start->prev = NULL;
  memory_start->next = after_hole;
  memory_start->size = HOLESTART - freemem - MEMORY_HEADER_SIZE;
  
  after_hole->prev = memory_start;
  after_hole->next = NULL;
  after_hole->size = maxaddr - HOLEEND - MEMORY_HEADER_SIZE;

  memory_debug();
}

void memory_debug() {
  kprintf("\n\nMemory Linked list");
  kprintf("\nSize of MemoryHeader is %ld", MEMORY_HEADER_SIZE); 
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
extern void *kmalloc( int size ) {
  kprintf("\nkmalloc: looking for %ld bytes", size);
  MemoryHeader *mem = memory_start;
  while (size > mem->size) {
    if (mem->next == NULL) { 
      kprintf("\nOut of memory!");
      return;
    }
    else mem = mem->next;
  }
  kprintf("\nkmalloc: found at location %ld", mem);
   
  // Create free memory header for remaining memory if sufficient
  if (mem->size - size >= MEMORY_HEADER_SIZE + 1204) {
    kprintf("\nkmalloc: some memory remains");
    MemoryHeader *remaining = mem->data_start + size + 1;
    remaining->size = mem->size - size - MEMORY_HEADER_SIZE;
    
    remaining->next = mem->next;
    remaining->prev = mem->prev;
    
    if (mem->prev != NULL) ((MemoryHeader *)(mem->prev))->next = remaining;
    if (mem->next != NULL) ((MemoryHeader *)(mem->next))->prev = remaining;

    if (memory_start == mem) memory_start = remaining;
  }
  else {
    kprintf("\nkmalloc: no extra memory remains");
    if (mem->prev != NULL) ((MemoryHeader *)(mem->prev))->next = mem->next;
    if (mem->next != NULL) ((MemoryHeader *)(mem->next))->prev = mem->prev;
    
    if (memory_start == mem) memory_start = mem->next;
  }
  mem->sanity_check = size;

  memory_debug();
  return mem->data_start;
}

extern void kfree( void *ptr ) {

}
