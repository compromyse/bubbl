#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <libk/string.h>

#include <driver/vga_text_buffer.h>

uint8_t vga_text_buffer_row;
uint8_t vga_text_buffer_column;
uint8_t vga_text_buffer_color;
uint16_t *vga_text_buffer_buffer;
bool vga_text_buffer_initialized = false;

static uint8_t
vga_entry_color(vga_color fg, vga_color bg)
{
    /*
     * bg   fg
     * 1110 0101
     */
    return bg << 4 | fg;
}

static uint16_t
vga_entry(unsigned char character, uint8_t color)
{
    /*
     * color     character
     * 1110 0101 1001 1010
     */
    return (uint16_t) color << 8 | (uint16_t) character;
}

bool
vga_text_buffer_is_initialized(void)
{
    return vga_text_buffer_initialized;
}

static void
vga_text_buffer_write_entry_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
    size_t index = y * VGA_WIDTH + x;
    vga_text_buffer_buffer[index] = vga_entry(c, color);
}

void
vga_text_buffer_initialize(void)
{
    vga_text_buffer_row = 0;
    vga_text_buffer_column = 0;
    vga_text_buffer_color = vga_entry_color(
	VGA_COLOR_LIGHT_GREY,
	VGA_COLOR_BLACK
    );
    vga_text_buffer_buffer = (uint16_t *) 0xB8000;

    for (uint8_t y = 0; y < VGA_HEIGHT; y++)
	for (uint8_t x = 0; x < VGA_WIDTH; x++)
	    vga_text_buffer_write_entry_at(' ', vga_text_buffer_color, x, y);

    vga_text_buffer_initialized = true;
}

void
vga_text_buffer_write_char(char c)
{
    if (c == '\n') {
	vga_text_buffer_row++;
	vga_text_buffer_column = 0;
    } else {
	vga_text_buffer_write_entry_at(c, vga_text_buffer_color, vga_text_buffer_column, vga_text_buffer_row);

	if (++vga_text_buffer_column == VGA_WIDTH) {
	    vga_text_buffer_column = 0;
	    if (++vga_text_buffer_row == VGA_HEIGHT)
		vga_text_buffer_row = 0;
	}
    }
}

void
vga_text_buffer_write_string(char *data)
{
    size_t size = strlen(data);
    for (size_t i = 0; i < size; i++)
	vga_text_buffer_write_char(data[i]);
}
