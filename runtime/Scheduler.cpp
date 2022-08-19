#include <Scheduler.h>
#include <CGA.h>
#include <Memory.h>
#include <lib.h>
#include <PIT.h>
#include <Serial.h>
thread_t *Scheduler::threads[512];
thread_t *Scheduler::runningThreads[512];

bool Scheduler::enabled = false;
bool Scheduler::isScheduling = false;

uint16_t Scheduler::threadCount = 0;
uint16_t Scheduler::currentThreadIndex = 0;
uint16_t Scheduler::processCount = 0;
uint16_t Scheduler::runningThreadCount = 0;

process_t *Scheduler::processes[512];

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
		threads[threadCount - 1] = nullptr;
		threadCount--;

		if (Serial::verboseOn)
			Serial::log("Scheduler setup [Done]\n");
	}
	thread_t *create(pdata_t *data, void *func)
	{
		if (threadCount >= MAX_THREADS)
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
		threads[threadCount] = out;
		threadCount++;
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
		for (uint16_t i = 0; i < threadCount; i++)
		{

			if (threads[i]->state == RUNNING)
			{
				runningThreads[runningThreadCount] = threads[i];
				runningThreadCount++;
			}
			else if (threads[i]->state == SLEEPING)
			{

				if (threads[i]->sleepTimeLeft <= timePassedSinceReschedule)
				{
					threads[i]->state = RUNNING;
				}
				else
					threads[i]->sleepTimeLeft -= timePassedSinceReschedule;
			}
			else if (threads[i]->state == KILL_REQUESTED)
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
		threads[index]->state = KILLED;
		free(threads[index]->stack);
		free((char *)threads[index]->regs);
		free((char *)threads[index]);
		//Bringing each thread one index back
		for (uint16_t i = index; i < threadCount; i++)
		{
			threads[i] = threads[i + 1];
		}
		threadCount--;
	}
	uint16_t allocPID()
	{
		return processCount + 1;
	}
	process_t *createProcess(string name)
	{

		process_t *out = (process_t *)malloc(sizeof(process_t));
		out->name = name;
		out->pid = allocPID();
		out->childrenCount = 0;
		processes[processCount] = out;
		processCount++;
		return out;
	}
	process_t *createProcess(string name, pdata_t *data, void *func)
	{

		if (processCount >= MAX_PROCESSES)
			return nullptr;
		process_t *out = createProcess(name);
		out->data = data;
		out->keyboardStream = StreamManager::CreateStream(1024);
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
		for (uint32_t i = 0; i < processCount; i++)
		{
			//Checking if the i'th process is proc or not
			if (processes[i] == proc)
			{
				//Bringing each process back by one index
				for (uint32_t j = i; j < processCount; j++)
				{
					processes[i] = processes[i + 1];
				}
				break;
			}
		}
		processCount--;
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
		return processes[processCount - 1];
	}
};
