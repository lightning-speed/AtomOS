#include <Runtime.h>
#include <ELF.h>
#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
#include <VFS.h>

namespace Runtime
{
	process_t *exec(string path, int argc, char **argv, char **env)
	{
		fnode *file = VFS::open(path.buffer, "r");
		if (file == nullptr)
			return nullptr;
		ELF_prog *prog = ELF::load(file);
		if (prog == nullptr)
		{
			Serial::log("\nProgram Execution Aborted!\n");
			VFS::close(file);
			return nullptr;
		}
		VFS::close(file);
		pdata_t *data = (pdata_t *)malloc(sizeof(pdata_t));
		data->argc = argc;
		data->argv = argv;
		data->env = env;
		process_t *out = Scheduler::createProcess(prog->name, data, prog->entryPoint);

		free((char *)prog);

		return out;
	}
	void kill(process_t *proc)
	{
		free((char *)proc->data);
		Scheduler::killProcess(proc);
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
		CGA::print((string) "In Interrupt: " + (uint32_t)inInterrupt + "\n");
		CGA::print((string) "Is Schedling: " + (uint32_t)(Scheduler::isScheduling) + "\n");

		CGA::print("Process: " + ((process_t *)ct->parent)->name + "\n");
		CGA::print((string) "EIP: " + regs->eip + "\n");
		CGA::print((string) "ESP: " + regs->esp + "\n");

		struct stackframe *stk;
		asm volatile("mov %%ebp, %0"
					 : "=r"(stk)::"memory");
		CGA::print("Stack trace:\n");
		CGA::print((string) "[" + regs->eip + "]" + "\n");

		for (unsigned int frame = 0; stk != nullptr; ++frame)
		{
			CGA::print((string) "[" + stk->eip + "]" + "\n");
			stk = stk->ebp;
		}
		Serial::log("\n");
		Serial::log((string) "In Interrupt: " + (uint32_t)inInterrupt + "\n");
		Serial::log((string) "Is Schedling: " + (uint32_t)(Scheduler::isScheduling) + "\n");

		Serial::log("Process: " + ((process_t *)ct->parent)->name + "\n");
		Serial::log((string) "EIP: " + regs->eip + "\n");
		Serial::log((string) "ESP: " + regs->esp + "\n");

		asm volatile("mov %%ebp, %0"
					 : "=r"(stk)::"memory");
		Serial::log("Stack trace:\n");
		Serial::log((string) "[" + regs->eip + "]" + "\n");

		for (unsigned int frame = 0; stk != nullptr; ++frame)
		{
			Serial::log((string) "[" + stk->eip + "]" + "\n");
			stk = stk->ebp;
		}
	}
}