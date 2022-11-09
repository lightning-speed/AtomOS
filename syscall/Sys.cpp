#include "Sys.h"
#include <CGA.h>
#include <Keyboard.h>
#include <FS.h>
#include <Runtime.h>
#include <Memory.h>
#include <CMOS.h>
#include <VFS.h>
#include "../Glib/WindowManager.h"
#include <Serial.h>
#include <FB.h>
#include <Mouse.h>
#include <RMm.h>
#include <SysInterface.h>
namespace Sys

{
	void exit(register_t *regs)
	{
	  ProcessManager::kill(((process_t*)Scheduler::getCurrentThread()->parent),regs->ebx);	
	}
	void read(register_t *regs)
	{
		uint32_t fdnum = regs->ebx;
		char * buff = (char *)regs->ecx;
		size_t count = regs->edx;
		SysInterface::syscall_read(fdnum,buff,count);	
	}
	void write(register_t *regs){
		uint32_t fdnum = regs->ebx;
		const char * buff = (const char *)regs->ecx;
		size_t count = regs->edx;
		regs->ecx = SysInterface::syscall_write(fdnum,buff,count);
	}
	void open(register_t *regs)
	{
		uint32_t fdnum = SysInterface::syscall_open((char *)regs->ebx,(int)regs->ecx,(short)regs->edx);
		regs->ecx = fdnum;	
	}
	void close(register_t *regs)
	{
		
	}
	
	void time(register_t *regs)
	{
		
	}
};