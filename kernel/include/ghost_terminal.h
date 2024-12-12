#ifndef __GHOST_TERMINAL_H
#define __GHOST_TERMINAL_H

#include "system.h"
#include "ghost_vga.h"  // For HDMI functions

/* Terminal Dimensions */
#define TERM_WIDTH      80
#define TERM_HEIGHT     25
#define TAB_SIZE        4

/* Terminal Colors (ARGB8888) */
#define TERM_COLOR_BLACK        0xFF000000
#define TERM_COLOR_RED         0xFFFF0000
#define TERM_COLOR_GREEN       0xFF00FF00
#define TERM_COLOR_YELLOW      0xFFFFFF00
#define TERM_COLOR_BLUE        0xFF0000FF
#define TERM_COLOR_MAGENTA     0xFFFF00FF
#define TERM_COLOR_CYAN        0xFF00FFFF
#define TERM_COLOR_WHITE       0xFFFFFFFF
#define TERM_COLOR_BRIGHT_BLACK    0xFF808080
#define TERM_COLOR_DEFAULT_FG      TERM_COLOR_WHITE
#define TERM_COLOR_DEFAULT_BG      TERM_COLOR_BLACK

/* Terminal Attributes */
#define TERM_ATTR_BOLD        0x01
#define TERM_ATTR_DIM         0x02
#define TERM_ATTR_UNDERLINE   0x04
#define TERM_ATTR_BLINK       0x08
#define TERM_ATTR_REVERSE     0x10

/* Terminal State */
typedef struct {
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t fg_color;
    uint32_t bg_color;
    uint8_t  attributes;
    bool     cursor_visible;
    bool     wrap_mode;
    char     buffer[TERM_HEIGHT][TERM_WIDTH];
    uint32_t fg_colors[TERM_HEIGHT][TERM_WIDTH];
    uint32_t bg_colors[TERM_HEIGHT][TERM_WIDTH];
    uint8_t  attr_map[TERM_HEIGHT][TERM_WIDTH];
} terminal_state_t;

/* Function Declarations */
extern void term_init(void);
extern void term_clear(void);
extern void term_write_char(char c);
extern void term_write_string(const char* str);
extern void term_set_cursor(uint32_t x, uint32_t y);
extern void term_show_cursor(bool visible);
extern void term_set_colors(uint32_t fg, uint32_t bg);
extern void term_set_attributes(uint8_t attrs);
extern void term_reset_attributes(void);
extern void term_scroll_up(void);
extern void term_handle_escape_sequence(const char* seq);
extern void term_render(void);

/* ANSI Escape Sequence Handling */
extern void term_parse_csi_sequence(const char* seq);
extern void term_set_mode(const char* params);
extern void term_reset_mode(const char* params);

/* Debug Output */
extern void term_debug_print(const char* str);

#endif
