#pragma once
#include <stdint.h>
namespace IO
{
	uint8_t inb(uint16_t _port);
	void outb(uint16_t _port, uint8_t _data);
	void outw(uint16_t _port, uint16_t _data);
	uint16_t inw(uint16_t _port);
};
