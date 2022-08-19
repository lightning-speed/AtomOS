#pragma once
#include <stdint.h>
#include <FS.h>
#include <Scheduler.h>

extern "C" uint32_t process_memory;

typedef struct __attribute__((packed))
{
	process_t proc;
	thread_t threads[10];
	uint8_t tcount;
} processdump_t;

struct stackframe
{
	struct stackframe *ebp;
	uint32_t eip;
};

namespace Runtime
{
	process_t *exec(string path, int argc, char **argv, char **env);
	void clearThrash();
	void kill(process_t *proc);
	void fullTrace(thread_t *ct, register_t *regs);
	void stackTrace();
};
