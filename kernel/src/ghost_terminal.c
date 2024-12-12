#include "../include/ghost_terminal.h"
#include "../include/ghost_mini_uart.h"

static terminal_state_t term_state;
static const uint32_t FONT_WIDTH = 8;
static const uint32_t FONT_HEIGHT = 16;
static const uint8_t default_font_8x16[128][16] = {
    // Basic ASCII font data 8x16 - you can replace this with your preferred font
    // For now using a simple 8x16 font for ASCII characters 0-127
    // ... font data here ...
};

void term_init(void) {
    // Initialize terminal state
    term_state.cursor_x = 0;
    term_state.cursor_y = 0;
    term_state.fg_color = TERM_COLOR_DEFAULT_FG;
    term_state.bg_color = TERM_COLOR_DEFAULT_BG;
    term_state.attributes = 0;
    term_state.cursor_visible = true;
    term_state.wrap_mode = true;

    // Clear terminal buffer
    term_clear();
    
    // Initialize UART for debug output
    uart_init();
}

void term_clear(void) {
    for (uint32_t y = 0; y < TERM_HEIGHT; y++) {
        for (uint32_t x = 0; x < TERM_WIDTH; x++) {
            term_state.buffer[y][x] = ' ';
            term_state.fg_colors[y][x] = term_state.fg_color;
            term_state.bg_colors[y][x] = term_state.bg_color;
            term_state.attr_map[y][x] = term_state.attributes;
        }
    }
    term_render();
}

void term_write_char(char c) {
    // Handle special characters
    switch (c) {
        case '\n':
            term_state.cursor_x = 0;
            if (term_state.cursor_y < TERM_HEIGHT - 1) {
                term_state.cursor_y++;
            } else {
                term_scroll_up();
            }
            break;
            
        case '\r':
            term_state.cursor_x = 0;
            break;
            
        case '\t':
            term_state.cursor_x = (term_state.cursor_x + TAB_SIZE) & ~(TAB_SIZE - 1);
            if (term_state.cursor_x >= TERM_WIDTH) {
                term_state.cursor_x = 0;
                if (term_state.cursor_y < TERM_HEIGHT - 1) {
                    term_state.cursor_y++;
                } else {
                    term_scroll_up();
                }
            }
            break;
            
        case '\b':
            if (term_state.cursor_x > 0) {
                term_state.cursor_x--;
            }
            break;
            
        default:
            if (c >= 32 && c < 127) {
                term_state.buffer[term_state.cursor_y][term_state.cursor_x] = c;
                term_state.fg_colors[term_state.cursor_y][term_state.cursor_x] = term_state.fg_color;
                term_state.bg_colors[term_state.cursor_y][term_state.cursor_x] = term_state.bg_color;
                term_state.attr_map[term_state.cursor_y][term_state.cursor_x] = term_state.attributes;
                
                term_state.cursor_x++;
                if (term_state.cursor_x >= TERM_WIDTH) {
                    if (term_state.wrap_mode) {
                        term_state.cursor_x = 0;
                        if (term_state.cursor_y < TERM_HEIGHT - 1) {
                            term_state.cursor_y++;
                        } else {
                            term_scroll_up();
                        }
                    } else {
                        term_state.cursor_x = TERM_WIDTH - 1;
                    }
                }
            }
            break;
    }
    
    // Render the changes
    term_render();
}

void term_write_string(const char* str) {
    while (*str) {
        if (*str == '\033') {
            // Handle escape sequence
            const char* seq_start = str + 1;
            while (*str && !isalpha(*str)) str++;
            if (*str) {
                char seq[32];
                size_t len = str - seq_start + 1;
                if (len < sizeof(seq)) {
                    memcpy(seq, seq_start, len);
                    seq[len] = '\0';
                    term_handle_escape_sequence(seq);
                }
            }
        } else {
            term_write_char(*str);
        }
        str++;
    }
}

void term_scroll_up(void) {
    // Move all lines up
    for (uint32_t y = 0; y < TERM_HEIGHT - 1; y++) {
        for (uint32_t x = 0; x < TERM_WIDTH; x++) {
            term_state.buffer[y][x] = term_state.buffer[y + 1][x];
            term_state.fg_colors[y][x] = term_state.fg_colors[y + 1][x];
            term_state.bg_colors[y][x] = term_state.bg_colors[y + 1][x];
            term_state.attr_map[y][x] = term_state.attr_map[y + 1][x];
        }
    }
    
    // Clear bottom line
    for (uint32_t x = 0; x < TERM_WIDTH; x++) {
        term_state.buffer[TERM_HEIGHT - 1][x] = ' ';
        term_state.fg_colors[TERM_HEIGHT - 1][x] = term_state.fg_color;
        term_state.bg_colors[TERM_HEIGHT - 1][x] = term_state.bg_color;
        term_state.attr_map[TERM_HEIGHT - 1][x] = term_state.attributes;
    }
}

void term_render(void) {
    // Clear the screen
    hdmi_clear_screen(TERM_COLOR_DEFAULT_BG);
    
    // Calculate terminal area dimensions
    uint32_t term_pixel_width = TERM_WIDTH * FONT_WIDTH;
    uint32_t term_pixel_height = TERM_HEIGHT * FONT_HEIGHT;
    uint32_t start_x = (SCREEN_WIDTH - term_pixel_width) / 2;
    uint32_t start_y = (SCREEN_HEIGHT - term_pixel_height) / 2;
    
    // Render each character
    for (uint32_t y = 0; y < TERM_HEIGHT; y++) {
        for (uint32_t x = 0; x < TERM_WIDTH; x++) {
            char c = term_state.buffer[y][x];
            if (c >= 32 && c < 127) {
                uint32_t char_x = start_x + (x * FONT_WIDTH);
                uint32_t char_y = start_y + (y * FONT_HEIGHT);
                uint32_t fg = term_state.fg_colors[y][x];
                uint32_t bg = term_state.bg_colors[y][x];
                
                // Draw character background
                hdmi_draw_rect(char_x, char_y, FONT_WIDTH, FONT_HEIGHT, bg);
                
                // Draw character using font data
                const uint8_t* char_data = default_font_8x16[c];
                for (uint32_t py = 0; py < FONT_HEIGHT; py++) {
                    uint8_t row = char_data[py];
                    for (uint32_t px = 0; px < FONT_WIDTH; px++) {
                        if (row & (0x80 >> px)) {
                            hdmi_draw_pixel(char_x + px, char_y + py, fg);
                        }
                    }
                }
            }
        }
    }
    
    // Draw cursor if visible
    if (term_state.cursor_visible) {
        uint32_t cursor_x = start_x + (term_state.cursor_x * FONT_WIDTH);
        uint32_t cursor_y = start_y + (term_state.cursor_y * FONT_HEIGHT);
        hdmi_draw_rect(cursor_x, cursor_y + FONT_HEIGHT - 2, FONT_WIDTH, 2, TERM_COLOR_DEFAULT_FG);
    }
}

void term_debug_print(const char* str) {
    // Send to UART for debugging
    uart_puts(str);
}
