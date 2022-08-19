#pragma once
#include <stdint.h>
#include <IDT.h>

namespace Syscall
{
	extern void *syscall_functions[];
	extern uint16_t syscall_functions_count;
	void init();
	void handler(register_t *regs);
	void addSyscall(uint16_t call_number, void *function);
};