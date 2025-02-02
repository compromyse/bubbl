.global _GDT_flush
.type _GDT_flush, @function
_GDT_flush:
  /* First Argument (Pointer to the GDT) */
  movl 4(%esp), %eax
  
  /* Load GDT */
  lgdt (%eax)

  /* Offset For Kernel Data Segment (16 bits) */
  mov $0x10, %eax

  /* Set the Data Segment Selectors */
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  /* Set the Code Segment Selector */
  jmp $0x08, $.flush

.flush:
  ret
