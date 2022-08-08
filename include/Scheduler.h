#pragma once
#include <stdint.h>
#include <IDT.h>
#include <String.h>
#include <Stream.h>

#define THREAD_STACK_SIZE 0x10000
#define MAX_THREADS 512
#define MAX_PROCESSES 512
enum state_t
{
	RUNNING = 1,
	SLEEPING = 2,
	KILLED = 3,
	PAUSED = 4,
	KILL_REQUESTED = 5
};
enum priority_t
{
	LOW = 0,
	MEDIUM = 1,
	HIGH = 2
};
typedef struct
{
	register_t *regs;
	char *stack;
	state_t state;
	uint64_t sleepTimeLeft;
	priority_t priority;
	void *parent;
} thread_t;
typedef struct
{
	int argc;
	char **argv;
	char **env;
} pdata_t;
typedef struct
{
	String name;
	uint16_t pid;
	thread_t *children[40];
	uint8_t childrenCount;
	pdata_t *data;
	Stream *keyboardStream;

} process_t;

namespace Scheduler
{
	extern thread_t *threads[];
	extern thread_t *runningThreads[];
	extern process_t *processes[];
	extern uint16_t currentThreadIndex;
	extern uint64_t timePassedSinceReschedule;
	extern uint16_t processCount;
	extern uint16_t threadCount;
	extern uint16_t runningThreadCount;
	extern int CPUUsage;

	void start();
	void idle();
	thread_t *create(pdata_t *data, void *func);
	void kill(thread_t *thread);
	void sleep(thread_t *thread, uint32_t milliseconds);
	void schedule(register_t *regs, uint64_t timePassed);
	void reSchedule();
	void sleepThread(thread_t *thread, uint64_t milliseconds);
	thread_t *getCurrentThread();
	void removeThreadAt(uint16_t index);
	void killThread(thread_t *thread);
	uint16_t allocPID();
	process_t *createProcess(String name);
	process_t *createProcess(String name, pdata_t *data, void *func);
	void addThreadToProcess(thread_t *thread, process_t *proc);
	void pauseThread(thread_t *t);
	void unpauseThread(thread_t *t);
	void killProcess(process_t *proc);
	extern bool enabled;
};