/* user.c : User processes
 */

#include <xeroskernel.h>

extern void root (void) {
  kprintf("\nHello World!");
  while (TRUE) {
    sysyield();
  }
};
