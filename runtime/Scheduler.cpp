#include <Scheduler.h>
#include <CGA.h>
#include <Memory.h>
#include <PIT.h>
#include <Serial.h>

//NOT USING LIST FOR THREADS FOR FAST ACCESS
List Scheduler::threads;
List Scheduler::processes;

thread_t *Scheduler::runningThreads[512];

bool Scheduler::enabled = false;
bool Scheduler::isScheduling = false;

uint16_t Scheduler::currentThreadIndex = 0;
uint16_t Scheduler::runningThreadCount = 0;

uint64_t Scheduler::timePassedSinceReschedule = 0;

int Scheduler::CPUUsage;

thread_t *idle_thread;

namespace Scheduler
{
	void idle()
	{
		for (;;)
			asm volatile("hlt");
	}
	void start()
	{

		enabled = true;
		PIT::init();
		idle_thread = create(nullptr, (void *)idle);
		//Removes the idle thread As we will only need it when there is not thread to run
		threads.pop();

		if (Serial::verboseOn)
			Serial::log("Scheduler setup [Done]\n");
	}
	thread_t *create(pdata_t *data, void *func)
	{
		if (threads.size >= MAX_THREADS)
			return nullptr;
		thread_t thread;
		//YES WE DO THIS
		thread.stack = malloc(THREAD_STACK_SIZE);
		thread.state = RUNNING;
		thread.priority = HIGH;
		thread.regs = (register_t *)malloc(sizeof(register_t));
		thread.regs->eax = (uint32_t)data;
		thread.regs->cs = 0x8;
		thread.regs->ss = 0x20;
		thread.regs->eflags = 0x202;

		//and THIS LINE IS USELESS, But I like it and therefore I left it there only
		thread.regs->int_no = 0;
		thread.regs->eip = (uint32_t)func;
		thread.regs->esp = (uint32_t)thread.stack + THREAD_STACK_SIZE;
		thread_t *out = (thread_t *)kernel_clone(&thread, sizeof(thread_t));
		threads.push(out);
		if (!enabled)
			reSchedule();
		return out;
	}

	void schedule(register_t *regs, uint64_t timePassed)
	{

		//Saves the register state

		*(runningThreads[currentThreadIndex]->regs) = *regs;

		//Change the thread index to next thread

		currentThreadIndex++;

		//Reschedule if end or the array is reached

		if (currentThreadIndex == runningThreadCount)
		{
			reSchedule();
		}

		//Set the Int regs to thread's reg
		*regs = *(runningThreads[currentThreadIndex]->regs);

		//ADDs  (time passed since last interrupt call) to the total count of time passed since last reschedule

		timePassedSinceReschedule += timePassed;
	}
	void reSchedule()
	{
		runningThreadCount = 0;
		uint16_t len = threads.size;
		for (uint16_t i = 0; i < len; i++)
		{
			thread_t *thread = (thread_t *)threads.at(i);
			if (thread->state == RUNNING)
			{
				runningThreads[runningThreadCount] = thread;
				runningThreadCount++;
			}
			else if (thread->state == SLEEPING)
			{

				if (thread->sleepTimeLeft <= timePassedSinceReschedule)
				{
					thread->state = RUNNING;
				}
				else
					thread->sleepTimeLeft -= timePassedSinceReschedule;
			}
			else if (thread->state == KILL_REQUESTED)
			{

				removeThreadAt(i);
			}
		}
		if (runningThreadCount == 0)
		{
			runningThreads[0] = idle_thread;
			runningThreadCount++;
		}
		runningThreads[runningThreadCount] = (thread_t *)nullptr;
		currentThreadIndex = 0;
		timePassedSinceReschedule = 0;
	}
	thread_t *getCurrentThread()
	{
		return runningThreads[currentThreadIndex];
	}
	void sleepThread(thread_t *thread, uint64_t milliseconds)
	{
		//10 millisecond is the avg time for watiting for next pit int call
		if (thread == nullptr)
			return;
		if (milliseconds > 10)
			milliseconds -= 10;
		thread->sleepTimeLeft = milliseconds;
		thread->state = SLEEPING;
	}

	void killThread(thread_t *thread)
	{
		if (thread != nullptr)
			thread->state = KILL_REQUESTED;
	}
	void removeThreadAt(uint16_t index)
	{
		thread_t *thread = ((thread_t *)threads.at(index));
		thread->state = KILLED;
		free(thread->stack);
		free((char *)thread->regs);
		free((char *)thread);
		threads.removeAt(index);
	}
	uint16_t allocPID()
	{
		return processes.size + 1;
	}
	process_t *createProcess(string name)
	{

		process_t *out = (process_t *)malloc(sizeof(process_t));
		out->name = name;
		out->pid = allocPID();
		out->childrenCount = 0;
		processes.push(out);
		return out;
	}
	process_t *createProcess(string name, pdata_t *data, void *func)
	{

		if (processes.size >= MAX_PROCESSES)
			return nullptr;
		process_t *out = createProcess(name);
		out->data = data;
		out->keyboardStream = StreamManager::CreateStream(20);
		out->mouseStream = StreamManager::CreateStream(50);
		addThreadToProcess(create(data, func), out);
		Serial::log("Created Porcess [" + name + "]\n");
		return out;
	}

	void killProcess(process_t *proc)
	{
		disableInt();
		if (proc == nullptr)
			return;
		StreamManager::DestoryStream(proc->keyboardStream);
		StreamManager::DestoryStream(proc->mouseStream);

		for (int i = 0; i < proc->childrenCount; i++)
		{
			proc->children[i]->state = KILL_REQUESTED;
		}
		processes.remove(proc);
		free((char *)proc);
		Serial::log("Killed Porcess [" + proc->name + "]\n");
		enableInt();
	}
	void addThreadToProcess(thread_t *thread, process_t *proc)
	{
		if (thread == nullptr)
			return;
		proc->children[proc->childrenCount] = thread;
		proc->childrenCount++;
		thread->parent = (void *)proc;
	}
	process_t *parentProcess(thread_t *thread)
	{
		return (process_t *)thread->parent;
	}
	void pauseThread(thread_t *t)
	{
		t->state = PAUSED;
	}
	void unpauseThread(thread_t *t)
	{
		t->state = RUNNING;
	}
	process_t *activeProcess()
	{
		return (process_t *)processes.end();
	}
};
