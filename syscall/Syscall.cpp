#include <Syscall.h>
#include <CGA.h>
#include "Sys.h"
#include <Serial.h>

#define SYSCALLS_COUNT 14

void *Syscall::syscall_functions[64];
namespace Syscall
{
	void init()
	{

		set_interrupt_handler(0x40 - 32, (void *)Syscall::handler);
		addSyscall(0, (void *)Sys::exit);
		addSyscall(1, (void *)Sys::write);
		addSyscall(2, (void *)Sys::read);
		addSyscall(3, (void *)Sys::open);
		addSyscall(4, (void *)Sys::close);
		addSyscall(5, (void *)Sys::exec);
		addSyscall(6, (void *)Sys::sleep);
		addSyscall(7, (void *)Sys::alloc);
		addSyscall(8, (void *)Sys::screenset);
		addSyscall(9, (void *)Sys::time);
		addSyscall(10, (void *)Sys::env);
		addSyscall(11, (void *)Sys::rem);
		addSyscall(12, (void *)Sys::wm);
	}
	extern inline void handler(register_t *regs)
	{
		uint32_t syscall_code = regs->eax;
		if (syscall_code < SYSCALLS_COUNT)
			((void (*)(register_t *))(void *)syscall_functions[syscall_code])(regs);
		else
		{
			CGA::print("Wrong syscall called");
			Serial::log("Wrong syscall called: " + ((string)(int)regs->eax) + " eip: " + (string)(int)regs->eip + "\n");
		}
	}
	void addSyscall(uint16_t call_number, void *function)
	{
		Syscall::syscall_functions[call_number] = (void *)function;
	}
}