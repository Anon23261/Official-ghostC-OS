#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stddef.h>
#include <stdint.h>

/* Type definitions */
typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

/* This defines what the stack looks like after an ISR was running */
struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

/* MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* CONSOLE.C */
extern void init_video(void);
extern void puts(const char *text);
extern void putch(unsigned char c);
extern void cls();

/* GDT.C */
extern void gdt_install(void);
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

/* IDT.C */
extern void idt_install(void);
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

/* ISRs.C */
extern void isrs_install(void);

/* IRQ.C */
extern void irq_install(void);
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);

/* TIMER.C */
extern void timer_install(void);
extern void timer_wait(int ticks);

/* KEYBOARD.C */
extern void keyboard_install(void);
extern char keyboard_getchar(void);

/* Memory Management */
extern void memory_install(void);
extern void *kmalloc(size_t size);
extern void kfree(void *ptr);

/* Paging */
extern void paging_install(void);

/* File System */
extern void fs_init(void);

/* Shell */
extern void shell_init(void);

/* System */
extern void panic(const char *message);
extern void reboot(void);
extern void shutdown(void);

#endif
