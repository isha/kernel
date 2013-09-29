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
  memory_start->prev = NULL;
  memory_start->next = NULL;
  *(memory_start->data_start) = (unsigned char *)freemem;
  memory_start->size = maxaddr - freemem - MEMORY_HEADER_SIZE;

  memory_debug();
}

void memory_debug() {
  kprintf("\n\nMemory Linked list");
  kprintf("\n-----------------------------");
  
  int i = 0;
  MemoryHeader *mheader = memory_start;
  
  do {
    kprintf("\nAt position 1 with address %ld ", mheader);
    kprintf("\n\tPrev = %ld, Next = %ld ", mheader->prev, mheader->next);
    kprintf("\n\tStarts at %ld with size %ld \n", mheader->data_start, mheader->size);

    mheader = mheader->next;
  } while (mheader != NULL);
    
}
extern void *kmalloc( int size ) {

}

extern void kfree( void *ptr ) {

}
