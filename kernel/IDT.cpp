#include <IDT.h>
#include <IO.h>
#include <CGA.h>
#include <Scheduler.h>
#include <Serial.h>
#include <Runtime.h>

void *interrupt_handlers[256];

static char *exception_messages[32] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(char vector, void *isr)
{
	struct idt_entry *descriptor = &idt[vector];

	descriptor->base_lo = (unsigned int)isr & 0xFFFF;
	descriptor->sel = 0x08;
	descriptor->flags = (char)0x8E;
	descriptor->base_hi = (unsigned int)isr >> 16;
	descriptor->always0 = 0;
}
void *memset(void *ptr, int value, int num)
{
	unsigned char *ptr_byte = (unsigned char *)ptr;

	for (int i = 0; i < num; ptr_byte[i] = (unsigned char)value, i++)
		;
	return ptr;
}
void idt_install()
{

	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (int)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	asm volatile("lidt %0" ::"m"(idtp));
}
void end_interrupt()
{
	IO::outb(MASTER_PIC_CMD_PORT, END_OF_INTERRUPT);
	IO::outb(SLAVE_PIC_CMD_PORT, END_OF_INTERRUPT);
}
void after_exception()
{
	CGA::print("\nThread Killed!\n\n");
	while (1)
		asm volatile("hlt");
}
extern "C" void interrupt_handler(register_t *regs)
{
	if (interrupt_handlers[regs->int_no] != nullptr)
		((void (*)(register_t *))(void *)interrupt_handlers[regs->int_no])(regs);
	end_interrupt();
}
extern "C" void exception_handler(register_t *regs)
{
	if (regs->int_no == 3)
	{
		end_interrupt();
		return;
	}
	CGA::print("\n\nException: ", 0x0c);
	CGA::print((String)exception_messages[regs->int_no]);
	CGA::print((String)(int)regs->eip);
	Serial::log("\n\nException: ");
	Serial::log((String)exception_messages[regs->int_no]);
	Serial::log((String)(int)regs->eip);
	if (regs->int_no == 13)
	{
		for (;;)
		{
		}
	}
	Runtime::kill((process_t *)(Scheduler::getCurrentThread()->parent));
	regs->eip = (uint32_t)after_exception;
	end_interrupt();
}
void set_interrupt_handler(int interrupt_code, void *handler)
{
	interrupt_handlers[interrupt_code] = handler;
}
void install_interrupt_gates()
{
	uint32_t space_between = (uint32_t)int1 - (uint32_t)int0;
	for (int i = 0; i < 60; i++)
	{
		idt_set_gate(32 + i, (void *)((uint32_t)int0 + (i * space_between)));
	}

	space_between = (uint32_t)ex1 - (uint32_t)ex0;
	for (int i = 0; i < 28; i++)
	{
		idt_set_gate(i, (void *)((uint32_t)ex0 + (i * space_between)));
	}
}
void enableInt()
{
	asm volatile("sti");
}
void disableInt()
{
	asm volatile("cli");
}