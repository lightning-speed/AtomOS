#pragma once
#include <stdint.h>
#include <IDT.h>
#include <String.h>
typedef uint32_t ComPort;

namespace Serial
{
	int serial_received();
	char read_serial();
	int is_transmit_empty();
	void write_serial(char a);
	void addPort(ComPort port);
	void log(String s);
	void handler(register_t *regs);
	void setup();
	extern bool verboseOn;
};