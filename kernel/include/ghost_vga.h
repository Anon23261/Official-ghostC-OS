#ifndef __GHOST_HDMI_H
#define __GHOST_HDMI_H

#include "system.h"

/* BCM2835 (Pi Zero W) VideoCore constants */
#define VIDEOCORE_BASE     0x20000000  // Base address for BCM2835
#define MAILBOX_BASE       (VIDEOCORE_BASE + 0xB880)
#define MAILBOX_READ       0x00
#define MAILBOX_STATUS     0x18
#define MAILBOX_WRITE      0x20
#define MAILBOX_EMPTY      0x40000000
#define MAILBOX_FULL       0x80000000

/* HDMI Configuration for Pi Zero W */
#define HDMI_POWER_DOMAIN  0x00000100  // Power domain for HDMI
#define HDMI_CLOCK_FREQ    432000000   // 432MHz pixel clock for HDMI
#define HDMI_FORCE_HOTPLUG 1           // Force HDMI hotplug detection

/* Frame Buffer Properties */
#define FB_CHANNEL         8
#define FB_ALIGN          16

/* Default Resolution - Pi Zero W supports up to 1080p */
#define SCREEN_WIDTH      1920
#define SCREEN_HEIGHT     1080
#define DEFAULT_DEPTH      32   // 32-bit color depth

/* Property Tags for VideoCore */
#define PROPTAG_GET_FIRMWARE_REV  0x00000001
#define PROPTAG_SET_CLOCK_RATE    0x00038002
#define PROPTAG_SET_POWER_STATE   0x00028001
#define PROPTAG_GET_BOARD_MODEL   0x00010001
#define PROPTAG_GET_BOARD_REV     0x00010002
#define PROPTAG_GET_MAC_ADDRESS   0x00010003

/* Frame Buffer Structure */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x_offset;
    uint32_t y_offset;
    uint32_t pointer;
    uint32_t size;
    uint16_t cmap[256];
} framebuffer_info_t;

/* HDMI Configuration Structure */
typedef struct {
    uint32_t clock_rate;
    uint32_t pixel_freq;
    uint8_t  hdmi_force;
    uint8_t  hdmi_boost;
    uint8_t  group;
    uint8_t  mode;
} hdmi_config_t;

/* Function Declarations */
extern int hdmi_init(void);
extern int hdmi_set_resolution(uint32_t width, uint32_t height, uint32_t depth);
extern void hdmi_set_power_state(int state);
extern int hdmi_get_edid_info(void);
extern void hdmi_draw_pixel(uint32_t x, uint32_t y, uint32_t color);
extern void hdmi_clear_screen(uint32_t color);
extern void hdmi_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
extern void hdmi_write_text(uint32_t x, uint32_t y, const char* text, uint32_t color);
extern int hdmi_check_connection(void);

/* Mailbox Interface */
extern uint32_t mailbox_read(uint8_t channel);
extern void mailbox_write(uint8_t channel, uint32_t data);
extern int mailbox_send(uint32_t* buffer, uint8_t channel);

#endif
