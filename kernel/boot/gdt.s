.intel_syntax noprefix

.global _GDT_flush
.type _GDT_flush, @function
_GDT_flush:
  /* First Argument (Pointer to the GDT) */
  mov eax, [esp + 4]
  
  /* Load GDT */
  lgdt [eax]

  /* Offset For Kernel Data Segment (16 bits) */
  mov eax, 0x10

  /* Set the Data Segment Selectors */
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  /* Set the Code Segment Selector */
  jmp 0x08:.flush

.flush:
  ret
