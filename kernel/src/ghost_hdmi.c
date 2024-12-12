#include "../include/ghost_vga.h"
#include "../include/system.h"

static volatile uint32_t* mailbox = (uint32_t*)MAILBOX_BASE;
static framebuffer_info_t fb_info __attribute__((aligned(16)));
static hdmi_config_t hdmi_cfg;
static uint32_t* framebuffer = NULL;

// Initialize HDMI configuration for Pi Zero W
static void init_hdmi_config(void) {
    hdmi_cfg.clock_rate = HDMI_CLOCK_FREQ;
    hdmi_cfg.pixel_freq = HDMI_CLOCK_FREQ;
    hdmi_cfg.hdmi_force = HDMI_FORCE_HOTPLUG;
    hdmi_cfg.hdmi_boost = 5;  // Medium boost level
    hdmi_cfg.group = 0;       // Auto-detect
    hdmi_cfg.mode = 0;        // Auto-detect
}

int hdmi_check_connection(void) {
    uint32_t __attribute__((aligned(16))) msg[8];
    
    msg[0] = 7 * 4;                  // Message size
    msg[1] = 0;                      // Request
    msg[2] = PROPTAG_GET_BOARD_MODEL;// Get board info
    msg[3] = 4;                      // Buffer size
    msg[4] = 0;                      // Request size
    msg[5] = 0;                      // Value buffer
    msg[6] = 0;                      // End tag
    
    mailbox_write(FB_CHANNEL, (uint32_t)msg);
    mailbox_read(FB_CHANNEL);
    
    return (msg[5] == 0x900093);     // Check if Pi Zero W
}

void hdmi_set_power_state(int state) {
    uint32_t __attribute__((aligned(16))) msg[8];
    
    msg[0] = 8 * 4;
    msg[1] = 0;
    msg[2] = PROPTAG_SET_POWER_STATE;
    msg[3] = 8;
    msg[4] = 8;
    msg[5] = HDMI_POWER_DOMAIN;
    msg[6] = state ? 1 : 0;
    msg[7] = 0;
    
    mailbox_write(FB_CHANNEL, (uint32_t)msg);
    mailbox_read(FB_CHANNEL);
}

int hdmi_init(void) {
    // Check if we're running on Pi Zero W
    if (!hdmi_check_connection()) {
        return -1;  // Not a Pi Zero W
    }
    
    // Initialize HDMI configuration
    init_hdmi_config();
    
    // Power on HDMI
    hdmi_set_power_state(1);
    
    // Setup framebuffer
    fb_info.width = SCREEN_WIDTH;
    fb_info.height = SCREEN_HEIGHT;
    fb_info.virtual_width = SCREEN_WIDTH;
    fb_info.virtual_height = SCREEN_HEIGHT;
    fb_info.depth = DEFAULT_DEPTH;
    fb_info.x_offset = 0;
    fb_info.y_offset = 0;
    
    // Create property message
    uint32_t __attribute__((aligned(16))) msg[32];
    
    msg[0] = 35 * 4;                 // Message size
    msg[1] = 0;                      // Request
    
    // Set physical WH
    msg[2] = PROPTAG_SET_PHYS_WH;
    msg[3] = 8;
    msg[4] = 8;
    msg[5] = fb_info.width;
    msg[6] = fb_info.height;
    
    // Set virtual WH
    msg[7] = PROPTAG_SET_VIRT_WH;
    msg[8] = 8;
    msg[9] = 8;
    msg[10] = fb_info.virtual_width;
    msg[11] = fb_info.virtual_height;
    
    // Set depth
    msg[12] = PROPTAG_SET_DEPTH;
    msg[13] = 4;
    msg[14] = 4;
    msg[15] = fb_info.depth;
    
    // Set pixel order
    msg[16] = 0x48006;
    msg[17] = 4;
    msg[18] = 4;
    msg[19] = 1;  // RGB, not BGR
    
    // Allocate buffer
    msg[20] = PROPTAG_ALLOCATE_BUFFER;
    msg[21] = 8;
    msg[22] = 8;
    msg[23] = 4096;  // Alignment
    msg[24] = 0;     // Space for response
    
    // Get pitch
    msg[25] = PROPTAG_GET_PITCH;
    msg[26] = 4;
    msg[27] = 4;
    msg[28] = 0;     // Space for response
    
    msg[29] = 0;     // End tag
    
    mailbox_write(FB_CHANNEL, (uint32_t)msg);
    mailbox_read(FB_CHANNEL);
    
    if (msg[1] != 0x80000000) {
        return -1;
    }
    
    fb_info.pointer = msg[23] & 0x3FFFFFFF;  // Convert to ARM physical address
    fb_info.size = msg[24];
    fb_info.pitch = msg[28];
    
    framebuffer = (uint32_t*)fb_info.pointer;
    
    return 0;
}

void hdmi_set_resolution(uint32_t width, uint32_t height, uint32_t depth) {
    fb_info.width = width;
    fb_info.height = height;
    fb_info.depth = depth;
    hdmi_init();
}

void hdmi_draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if(x >= fb_info.width || y >= fb_info.height || !framebuffer)
        return;
    
    uint32_t pixel_offset = y * (fb_info.pitch/4) + x;
    framebuffer[pixel_offset] = color;
}

void hdmi_clear_screen(uint32_t color) {
    if(!framebuffer)
        return;
        
    for(uint32_t y = 0; y < fb_info.height; y++) {
        for(uint32_t x = 0; x < fb_info.width; x++) {
            hdmi_draw_pixel(x, y, color);
        }
    }
}

void hdmi_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    for(uint32_t i = y; i < y + height && i < fb_info.height; i++) {
        for(uint32_t j = x; j < x + width && j < fb_info.width; j++) {
            hdmi_draw_pixel(j, i, color);
        }
    }
}
