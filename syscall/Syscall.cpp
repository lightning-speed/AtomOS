#include <Syscall.h>
#include <CGA.h>
#include "Sys.h"
#include <Serial.h>
#include <Scheduler.h>

#define SYSCALLS_COUNT 7

void *Syscall::syscall_functions[64];
namespace Syscall
{
	void init()
	{
		for(int i =0 ;i<64;i++){
			syscall_functions[i]= nullptr;
		}
		set_interrupt_handler(0x40 - 32, (void *)Syscall::handler);
		addSyscall(1, (void *)Sys::exit);
		addSyscall(3, (void *)Sys::read);
		addSyscall(4, (void *)Sys::write);
		addSyscall(5, (void *)Sys::open);
		addSyscall(6, (void *)Sys::close);
	}
	extern inline void handler(register_t *regs)
	{

		uint32_t syscall_code = regs->eax;
		
		if (syscall_code<64&&syscall_functions[syscall_code]!=nullptr){
			((void (*)(register_t *))(void *)syscall_functions[syscall_code])(regs);
		}
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