#pragma once
#include <stdint.h>
#include <FS.h>
#include <Scheduler.h>

extern "C" uint32_t process_memory;

namespace Runtime
{
	process_t *exec(String path, int argc, char **argv, char **env);
	void clearThrash();
	void kill(process_t *proc);
};
