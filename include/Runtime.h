#pragma once
#include <stdint.h>
#include <FS.h>
#include <Scheduler.h>
#include <Process.h>

extern "C" uint32_t process_memory;



struct stackframe
{
	struct stackframe *ebp;
	uint32_t eip;
};

namespace Runtime
{
	process_t *exec(string path, int argc, char **argv, char **env);
	void clearThrash();
	void kill(process_t *proc,int exit_code);
	void fullTrace(thread_t *ct, register_t *regs);
	void stackTrace();
};
