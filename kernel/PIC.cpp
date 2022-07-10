#include <PIC.h>
#include <IO.h>
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

namespace PIC
{
	using namespace IO;
	void init()
	{
		uint8_t a1, a2;

		a1 = inb(PIC1_DATA);
		io_wait();
		a2 = inb(PIC2_DATA);
		io_wait();

		outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
		io_wait();
		outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
		io_wait();

		outb(PIC1_DATA, 0x20);
		io_wait();
		outb(PIC2_DATA, 0x28);
		io_wait();

		outb(PIC1_DATA, 4);
		io_wait();
		outb(PIC2_DATA, 2);
		io_wait();

		outb(PIC1_DATA, ICW4_8086);
		io_wait();
		outb(PIC2_DATA, ICW4_8086);
		io_wait();

		outb(PIC1_DATA, a1);
		io_wait();
		outb(PIC2_DATA, a2);
	}
};