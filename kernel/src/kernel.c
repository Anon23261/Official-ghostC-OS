#include "../include/gpio.h"
#include "../include/uart.h"
#include "../include/timer.h"
#include "../include/ghost_mini_uart.h"
#include "../include/ghost_vga.h"
#include "../include/ghost_terminal.h"

// External assembly functions
extern void enable_irq(void);
extern void disable_irq(void);

// Define some colors
#define COLOR_BLACK   0x00000000
#define COLOR_WHITE   0xFFFFFFFF
#define COLOR_RED     0x00FF0000
#define COLOR_GREEN   0x0000FF00
#define COLOR_BLUE    0x000000FF

void kernel_main(void) {
    // Initialize hardware
    hdmi_init();
    term_init();
    
    // Display boot message
    term_write_string("\033[32mGhostC OS Bootloader v1.0\033[0m\n\n");
    
    const char* boot_stages[] = {
        "Initializing ARM processor...",
        "Loading system modules...",
        "Mounting ghost partitions...",
        "Starting security services...",
        "Initializing Rune environment...",
        "Loading ghost protocols...",
        "Establishing secure connections...",
        "Starting system services...",
        "Performing security checks...",
        "Finalizing boot sequence..."
    };
    
    int num_stages = sizeof(boot_stages) / sizeof(char*);
    
    // Display boot progress
    for (int i = 0; i < num_stages; i++) {
        int percentage = (i * 100) / num_stages;
        
        // Clear line and show progress
        term_write_string("\r\033[K"); // Clear line
        term_write_string("\033[36m["); // Cyan
        
        // Draw progress bar
        for (int j = 0; j < 50; j++) {
            if (j < (percentage / 2)) {
                term_write_string("=");
            } else if (j == (percentage / 2)) {
                term_write_string(">");
            } else {
                term_write_string(" ");
            }
        }
        
        term_write_string("] ");
        
        // Print percentage and stage
        char percent_str[8];
        sprintf(percent_str, "%3d%%", percentage);
        term_write_string(percent_str);
        term_write_string(" ");
        term_write_string("\033[0m"); // Reset color
        term_write_string(boot_stages[i]);
        
        // Debug output
        term_debug_print(boot_stages[i]);
        term_debug_print("\n");
        
        // Delay for visual effect
        timer_wait(100);
    }
    
    // Boot complete
    term_write_string("\n\n\033[32m"); // Green text
    term_write_string("   ▄██████▄     ▄█    █▄     ▄██████▄     ▄████████     ███      ▄████████ \n");
    term_write_string("  ███    ███   ███    ███   ███    ███   ███    ███ ▀█████████▄ ███    ███ \n");
    term_write_string("  ███    █▀    ███    ███   ███    ███   ███    █▀     ▀███▀▀██ ███    █▀  \n");
    term_write_string(" ▄███         ▄███▄▄▄▄███▄▄ ███    ███   ███            ███   ▀ ███        \n");
    term_write_string("▀▀███ ████▄  ▀▀███▀▀▀▀███▀  ███    ███ ▀███████████     ███     ███        \n");
    term_write_string("  ███    ███   ███    ███   ███    ███          ███     ███     ███    █▄  \n");
    term_write_string("  ███    ███   ███    ███   ███    ███    ▄█    ███     ███     ███    ███ \n");
    term_write_string("  ████████▀    ███    █▀     ▀██████▀   ▄████████▀     ▄████▀   ████████▀  \n");
    
    term_write_string("\033[36m"); // Cyan text
    term_write_string("\n                          by: GHOST Sec\n");
    term_write_string("\033[0m"); // Reset text color
    
    term_write_string("\nGhostC OS initialized successfully.\n");
    term_write_string("Type 'help' for available commands.\n\n");
    
    // Show cursor
    term_show_cursor(true);
    
    // Initialize system components
    timer_init();
    enable_irq();
    
    // Enter kernel loop
    while(1) {
        // Kernel main loop
        // TODO: Add command processing
    }
}
