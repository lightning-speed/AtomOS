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
}