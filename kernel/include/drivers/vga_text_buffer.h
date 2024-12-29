#ifndef __drivers_vga_text_buffer_h
#define __drivers_vga_text_buffer_h

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* Hardware text mode color constants. */
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

uint8_t vga_entry_color(vga_color fg, vga_color bg);
uint16_t vga_entry(unsigned char character, uint8_t color);

void terminal_initialize(void);
void terminal_write_entry_at(char c, uint8_t color, uint8_t x, uint8_t y);
void terminal_write_char(char c);
void terminal_write_string(char *data);

#endif
