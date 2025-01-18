#include <stdint.h>

#include <kernel/io.h>

/* Sends a 8/16/32-bit value on a I/O location. Traditional names are outb,
 * outw and outl respectively. The a modifier enforces val to be placed in the
 * eax register before the asm command is issued and Nd allows for one-byte
 * constant values to be assembled as constants, freeing the edx register for
 * other cases. */

void
outb(uint16_t port, uint8_t val)
{
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");

  /* There's an outb %al, $imm8 encoding, for compile-time constant port
   * numbers that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port  is a uint16_t.  %w1 could be used if we had the port number a wider
   * C type */
}

/* Receives a 8/16/32-bit value from an I/O location. Traditional names are
 * inb, inw and inl respectively. */

uint8_t
inb(uint16_t port)
{
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}
