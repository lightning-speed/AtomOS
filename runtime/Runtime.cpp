#include <Runtime.h>

#include <Process.h>
#include <ELF.h>
#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
#include <VFS.h>

namespace Runtime
{
	process_t *exec(string path, int argc, char **argv, char **env)
	{
		fnode * node = VFS::open(path.buffer,"r");
		if(node==nullptr)return nullptr;
		ELF_prog *prog = ELF::load(node);
		if(prog==nullptr)
			return nullptr;
		process_t * out = ProcessManager::create(path,argc,argv,(ENV_VARS*)env,prog->entryPoint);
		ProcessManager::start(out);
		return out;
	}
	void kill(process_t *proc,int exit_code)
	{
		ProcessManager::kill(proc,exit_code);
	}
	void clearThrash()
	{
		for (int i = 0; i < 0x600000; i++)
		{
			((char *)0x700000)[i] = 0;
		}
	}
	void stackTrace()
	{
		register uint32_t ebp asm("ebp");
		struct stackframe *stk = (struct stackframe *)ebp;
		stk = stk->ebp;
		CGA::print("Stack trace:\n");
		for (unsigned int frame = 0; stk; ++frame)
		{
			char *str = itoha(stk->eip);
			if (stk->eip != 0)

				CGA::print(" 0x" + (string)str + (string) "\n");
			free((str - 13));
			stk = stk->ebp;
		}
	}
	void fullTrace(thread_t *ct, register_t *regs)
	{
		CGA::print("\n");
		CGA::print("Process: " + (string)(((process_t *)ct->parent)->name) + "\n");
		CGA::print((string) "In Interrupt: " + (uint32_t)inInterrupt + "\n");
		CGA::print((string) "Is Schedling: " + (uint32_t)(Scheduler::isScheduling) + "\n");

		CGA::print((string) "EIP: " + regs->eip + "\n");
		CGA::print((string) "ESP: " + regs->esp + "\n");

		struct stackframe *stk = (struct stackframe*)regs->ebp;
		CGA::print("Stack trace:\n");
		CGA::print((string) "[" + regs->eip + "]" + "\n");

		for (unsigned int frame = 0; stk != nullptr; ++frame)
		{
			CGA::print((string) "[" + stk->eip + "]" + "\n");
			stk = stk->ebp;
		}
		CGA::print("Mem Used:"+ string(mem_used()/1024) + " KIB \n");
		
	}
}