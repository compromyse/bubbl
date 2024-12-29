#include <libk/io.h>
#include <driver/vga_text_buffer.h>

void
printk(char *msg)
{
    if (!vga_text_buffer_is_initialized())
      vga_text_buffer_initialize();

    vga_text_buffer_write_string(msg);
}
