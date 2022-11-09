#pragma once
#include <stdint.h>
#include <IDT.h>
#include <String.h>
#include <Stream.h>
#include <lib.h>

#define THREAD_STACK_SIZE 0x10000
#define MAX_THREADS 512
#define MAX_PROCESSES 512
enum state_t
{
	INITALIZED = 0,
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
	int header;
	register_t *regs;
	char *stack;
	state_t state;
	uint64_t sleepTimeLeft;
	priority_t priority;
	void *parent;
	bool locked;
	uint64_t errno;
} thread_t;
typedef struct
{
	int argc;
	char **argv;
	char **env;
} pdata_t;


namespace Scheduler
{
	extern List threads;
	extern bool isScheduling;
	extern List runningThreads;
	extern SpinLock spinLock;
	extern uint16_t currentThreadIndex;
	extern uint64_t timePassedSinceReschedule;
	extern uint16_t runningThreadCount;
	extern int CPUUsage;

	void start();
	void idle();
	thread_t *createThread(void * parentProcess,pdata_t *data, void *func);
	void kill(thread_t *thread);
	void sleep(thread_t *thread, uint32_t milliseconds);
	void schedule(register_t *regs, uint64_t timePassed);
	void reSchedule();
	void sleepThread(thread_t *thread, uint64_t milliseconds);
	thread_t *getCurrentThread();

	extern inline void sleep(uint32_t milliseconds)
	{

		thread_t *thread = getCurrentThread();

		if (thread == nullptr||thread->state !=RUNNING ||thread->locked == true)
			return;
		
		if (milliseconds > 10)
			milliseconds -= 10;
		thread->sleepTimeLeft = milliseconds;
		thread->state = SLEEPING;
		if (!inInterrupt)
			asm volatile("hlt");
	}
	void removeThread(thread_t * thread);
	void killThread(thread_t *thread);
	void selfKill();
	uint16_t allocPID();
	
	extern bool enabled;
};