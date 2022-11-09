#pragma once
#include <stdint.h>
#include <IDT.h>
#include <Scheduler.h>

namespace Sys
{
	void exit(register_t *regs);
	void write(register_t *regs);
	void read(register_t *regs);
	void open(register_t *regs);
	void close(register_t *regs);
};