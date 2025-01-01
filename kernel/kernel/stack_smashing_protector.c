#include <stdint.h>

#include <libk/io.h>

#include <kernel/halt.h>

/* TODO: Randomize */
#define STACK_CHK_GUARD 0xe2dee396

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void
__stack_chk_fail(void)
{
  /* TODO: Panic the kernel */
  printk("Stack Smashing Protector", "Stack smashing detected!");
  halt();
}
