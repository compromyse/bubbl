#include <libk/io.h>

#include <driver/vga_text_buffer.h>

void
kernel_main(void)
{
    vga_text_buffer_initialize();
    printk("kernel_main: Started\n");
}
