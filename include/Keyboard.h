#pragma once
#include <stdint.h>
#include <IDT.h>
namespace Keyboard
{
	void handler(register_t *regs);
	void init();
	extern int CharPressed;
};