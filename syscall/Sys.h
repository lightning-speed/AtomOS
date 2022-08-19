#pragma once
#include <stdint.h>
#include <IDT.h>

namespace Sys
{
	void exit(register_t *regs);
	void write(register_t *regs);
	void read(register_t *regs);
	void open(register_t *regs);
	void close(register_t *regs);
	void exec(register_t *regs);
	void sleep(register_t *regs);
	void alloc(register_t *regs);
	void screenset(register_t *regs);
	void time(register_t *regs);
	void env(register_t *regs);
	void rem(register_t *regs);
	void wm(register_t *regs);
	void pg(register_t *regs);
};