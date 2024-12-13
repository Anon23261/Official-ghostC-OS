#ifndef __GHOST_VGA_H
#define __GHOST_VGA_H

#include "system.h"
#include <stdint.h>

/* Hardware text mode color constants */
enum vga_color {
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
};

/* Mailbox message tags */
#define PROPTAG_SET_PHYS_WH     0x48003
#define PROPTAG_SET_VIRT_WH     0x48004
#define PROPTAG_SET_DEPTH       0x48005
#define PROPTAG_ALLOCATE_BUFFER 0x40001
#define PROPTAG_GET_PITCH       0x40008

/* Base address for mailbox */
#define MAILBOX_BASE 0x2000B880

/* HDMI Configuration for Pi Zero W */
#define HDMI_POWER_DOMAIN  0x00000100  // Power domain for HDMI
#define HDMI_CLOCK_FREQ    432000000   // 432MHz pixel clock for HDMI
#define HDMI_FORCE_HOTPLUG 1           // Force HDMI hotplug detection

/* HDMI-related constants */
#define HDMI_MODE_HDMI 0x00000002
#define HDMI_MODE_DVI  0x00000004
#define HDMI_MODE_NTSC 0x00000008
#define HDMI_MODE_PAL  0x00000010

/* Function declarations */
void vga_initialize(void);
void vga_setcolor(uint8_t color);
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);
void vga_putchar(char c);
void vga_write(const char* data, size_t size);
void vga_writestring(const char* data);
void vga_clear(void);

/* HDMI Functions */
void hdmi_init(void);
int hdmi_set_resolution(uint32_t width, uint32_t height, uint32_t depth);
void hdmi_draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void hdmi_clear_screen(uint32_t color);

#endif
