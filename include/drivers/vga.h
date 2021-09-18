#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define WHITE_ON_BLACK 15
#define RED_ON_WHITE 244

#define WHITE 0xF
#define YELLOW 0xE
#define PINK 0xD
#define LIGHT_RED 0xC
#define LIGHT_CYAN 0xB
#define LIGHT_GREEN 0xA
#define LIGHT_BLUE 0x9
#define DARK_GRAY 0x8
#define LIGHT_GRAY 0x7
#define MAGENTA 0x5
#define RED 0x4
#define CYAN 0x3
#define GREEN 0x2
#define BLUE 0x1
#define BLACK 0
#define BLACK_BG         0x00
#define BLUE_BG          0x10
#define GREEN_BG         0x20
#define CYAN_BG          0x30
#define RED_BG           0x40
#define MAGENTA_BG       0x50
#define BROWN_BG         0x60
#define LIGHT_GREY_BG    0x70

/*
#define BLACK            0x0000
#define BLUE             0x0100
#define GREEN            0x0200
#define CYAN             0x0300
#define RED              0x0400
#define MAGENTA          0x0500
#define BROWN            0x0600
#define LIGHT_GREY       0x0700
#define DARK_GREY        0x0800
#define LIGHT_BLUE       0x0900
#define LIGHT_GREEN      0x0A00
#define LIGHT_CYAN       0x0B00
#define LIGHT_RED        0x0C00
#define LIGHT_MAGENTA    0x0D00
#define LIGHT_BROWN      0x0E00
#define WHITE            0x0F00

#define BLACK_BG         0x0000
#define BLUE_BG          0x1000
#define GREEN_BG         0x2000
#define CYAN_BG          0x3000
#define RED_BG           0x4000
#define MAGENTA_BG       0x5000
#define BROWN_BG         0x6000
#define LIGHT_GREY_BG    0x7000
*/


int get_offset(int row, int col);
int get_offset_row(int offset);
int get_offset_col(int offset);
int get_cursor_offset();
void set_cursor_offset(int offset);
void clear_screen();
int put_char_at(char c, int row, int col, int color);
int put_char(char c);
char get_char_at(int row, int col);
void demo_graphics();
void set_attribute(uint16_t a);
void set_text_mode(int hi_res);
void init_vga();
static void write_font(unsigned char *buf, unsigned font_height);
static void write_pixel2(unsigned x, unsigned y, unsigned c);
#endif