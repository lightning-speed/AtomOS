#pragma once
#include <stdint.h>
#define MASTER_PIC_CMD_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21
#define SLAVE_PIC_CMD_PORT 0xA0
#define SLAVE_PIC_DATA_PORT 0xA1
#define END_OF_INTERRUPT 0x20

typedef struct
{
	uint32_t ebp, esi, edi, edx, ebx, ecx, eax;
	uint32_t int_no, eip, cs, eflags, esp, ss;
} register_t;

struct idt_entry
{
	unsigned short base_lo;

	unsigned short sel;
	unsigned char always0;
	unsigned char flags;
	unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));
void idt_set_gate(char vector, void *isr);
void idt_install();
void init_protection();
void end_interrupt();
void set_interrupt_handler(int interrupt_code, void *handler);
void install_interrupt_gates();
extern "C" void int0();
extern "C" void int1();
extern "C" void ex0();
extern "C" void ex1();
void enableInt();
void disableInt();

extern bool inInterrupt;