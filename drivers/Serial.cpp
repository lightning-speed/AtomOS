#include <Serial.h>
#include <CGA.h>
#include <IO.h>
using namespace IO;

ComPort CurrentPort = 0x3F8;
int serialx = 0;
bool Serial::verboseOn = false;
namespace Serial
{
	int serial_received()
	{
		return inb(CurrentPort + 5) & 1;
	}

	char read_serial()
	{
		while (serial_received() == 0)
			;

		return inb(CurrentPort);
	}
	int is_transmit_empty()
	{
		return inb(CurrentPort + 5) & 0x20;
	}

	void write_serial(char a)
	{
		while (is_transmit_empty() == 0)
			;

		outb(CurrentPort, a);
	}
	void addPort(ComPort CurrenPort)
	{
		outb(CurrentPort + 1, 0x00);
		outb(CurrentPort + 3, 0x80);
		outb(CurrentPort + 0, 0x01);
		outb(CurrentPort + 1, 0x00);
		outb(CurrentPort + 3, 0x03);
		outb(CurrentPort + 2, 0xC7);
		outb(CurrentPort + 4, 0x0B);
	}
	void handler(register_t *regs)
	{
		CGA::print(read_serial());
	}
	void log(string s)
	{
		uint32_t n = s.length();
		char *buffer = s.buffer;
		for (uint32_t i = 0; i < n; i++)
		{
			write_serial(buffer[i]);
		}
	}
	void setup()
	{
		addPort(CurrentPort);
		set_interrupt_handler(4, (void *)Serial::handler);
		outb(CurrentPort + 1, 0x01);
	}
};