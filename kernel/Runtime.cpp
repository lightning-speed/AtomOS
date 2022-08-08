#include <Runtime.h>
#include <ELF.h>
#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
#include <VFS.h>

namespace Runtime
{
	process_t *exec(String path, int argc, char **argv, char **env)
	{
		fnode *file;
		if (VFS::exists(path.buffer))
			file = VFS::open(path.buffer, "r");
		else
		{
			return nullptr;
		}
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

		/*struct stackframe *stk;
	
		Serial::log("Stack trace:\n");
		for (unsigned int frame = 0; stk && frame < 4; ++frame)
		{
			Serial::log((int)stk->eip + (String) "\n");
			stk = stk->ebp;
		}*/
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
			CGA::print(" 0x" + (String)str + (String) "\n");
			free((str - 13));
			stk = stk->ebp;
		}
	}
	void dumpProcess(process_t *proc, char *path)
	{
		Serial::log("Meow");

		processdump_t *pd = (processdump_t *)malloc(sizeof(processdump_t));
		memcpy((char *)&(pd->proc), (char *)proc, sizeof(process_t));
		for (uint8_t i = 0; i < proc->childrenCount; i++)
		{
			memcpy((char *)&(pd->threads[i]), (char *)proc->children[i], sizeof(thread_t));
		}
		pd->tcount = proc->childrenCount;
		fnode *outf = VFS::open(path, "w");
		VFS::setBuffer(outf, (char *)pd, sizeof(processdump_t));
		VFS::close(outf);
		Serial::log("Meow");

		Serial::log("Meow" + (String)outf->name);
	}
}