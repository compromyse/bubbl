#include <libk/io.h>
#include <driver/vga_text_buffer.h>

void
printk(char *msg)
{
    vga_text_buffer_write_string(msg);
}
