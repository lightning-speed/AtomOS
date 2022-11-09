#pragma once

#include <lib.h>
#include <VFS.h>
#include <Scheduler.h>
#include <String.h>

typedef struct {
	char * path;
	char * cwd;
	char * user_dir;
}ENV_VARS;

typedef struct
{
	char name[32];
	List threads;
	fnode * cwd;
	ENV_VARS env_vars;
	int argc;
	char ** argv;
	void * entry;
	uint32_t PID;
	state_t state;
	int exit_no;
} process_t;

namespace ProcessManager{
	uint32_t allocPID();
	process_t *create(string name,int argc,char ** argv,ENV_VARS *env_vars,void * entry);
	void start(process_t*proc);
	void addThread(process_t * proc,void * func);
	void attachThread(process_t * proc,thread_t * thread);
	void kill(process_t * proc,int exit_no);
	process_t * fromPID(uint32_t pid);
	void selfKill(int exit_no);

}