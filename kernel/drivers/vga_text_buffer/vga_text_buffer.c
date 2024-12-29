#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>

#include <drivers/vga_text_buffer.h>

uint8_t terminal_row;
uint8_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

uint8_t
vga_entry_color(vga_color fg, vga_color bg)
{
  /*
   * bg   fg
   * 1110 0101
  */
  return bg << 4 | fg;
}

uint16_t
vga_entry(unsigned char character, uint8_t color)
{
  /*
   * TODO: Unsure if this understanding
   * is correct.
   * color     character
   * 1110 0101 1001 1010
  */
  return (uint16_t) color << 8 | (uint16_t) character;
}

void
terminal_initialize(void)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_BLACK
  );
  terminal_buffer = (uint16_t *) 0xB8000;

  for (uint8_t y = 0; y < VGA_HEIGHT; y++)
    for (uint8_t x = 0; x < VGA_WIDTH; x++) {
      size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void
terminal_write_entry_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
  uint8_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void
terminal_write_char(char c)
{
  terminal_write_entry_at(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = 0;
  }
}

void
terminal_write_string(char *data)
{
  size_t size = strlen(data);
  for (size_t i = 0; i < size; i++)
    terminal_write_char(data[i]);
}

