#include <drivers/vga_text_buffer.h>

void
kernel_main(void)
{
    terminal_initialize();
    terminal_write_string("Testing");
}
