#include <Scheduler.h>
#include <CGA.h>
#include <Memory.h>
#include <PIT.h>
#include <Serial.h>

//NOT USING LIST FOR THREADS FOR FAST ACCESS
List Scheduler::threads;

SpinLock Scheduler::spinLock;

List Scheduler::runningThreads;

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
		idle_thread = createThread(nullptr,nullptr, (void *)idle);
		//Removes the idle thread As we will only need it when there is not thread to run
		threads.pop();
	}
	thread_t *createThread(void* parentProcess,pdata_t *data, void *func)
	{
		spinLock.acquire();
		
		if (threads.size >= MAX_THREADS)
			return nullptr;
		thread_t thread;
		thread.header = 0x6969;
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

		thread.locked = false;
		thread.parent = parentProcess;
		thread_t *out = (thread_t *)kernel_clone(&thread, sizeof(thread_t));
		
		threads.push(out);
		if (!enabled)
			reSchedule();
		spinLock.release();
		return out;
	}

	void schedule(register_t *regs, uint64_t timePassed)
	{

		//Saves the register state
		*(((thread_t *)runningThreads.at(currentThreadIndex))->regs) = *regs;

		//Change the thread index to next thread
		do{currentThreadIndex++;}
		while(((thread_t *)runningThreads.at(currentThreadIndex))->locked==true&&runningThreads.length()<currentThreadIndex);

		//Reschedule if end or the array is reached

		if (currentThreadIndex >= runningThreads.length())
		{
			reSchedule();
		}

		//Set the Int regs to thread's reg
		*regs = *(((thread_t *)runningThreads.at(currentThreadIndex))->regs);

		//ADDs  (time passed since last interrupt call) to the total count of time passed since last reschedule

		timePassedSinceReschedule += timePassed;
	}
	void reSchedule()
	{
		runningThreads.flush();
		for (uint16_t i = 0; i < (uint16_t)threads.length();  i++)
		{
			thread_t *thread = (thread_t *)threads.at(i);
			if(thread!=nullptr&&thread->regs!=nullptr){
				if (thread->state == RUNNING)
				{
					runningThreads.push(thread);
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
				else if(thread->state == KILL_REQUESTED){
					removeThread(thread);
					i--;
				}
			}
			
		}
		if (runningThreads.length()==0)
		{
			runningThreads.push(idle_thread);
		}
		currentThreadIndex = 0;
		timePassedSinceReschedule = 0;
	}
	thread_t *getCurrentThread()
	{
		return (thread_t*)runningThreads.at(currentThreadIndex);
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
		if(thread==nullptr||thread->locked||thread->state==KILL_REQUESTED)return;
		thread->state = KILL_REQUESTED;
		thread->locked = true;
		
	}
	void removeThread(thread_t * thread)
	{
		if(spinLock.locked==false){
			threads.remove(thread);
			thread->state = KILLED;
	    	}
	}
	
	void pauseThread(thread_t *t)
	{
		t->state = PAUSED;
	}
	void unpauseThread(thread_t *t)
	{
		t->state = RUNNING;
	}
	void selfKill(){
		killThread(Scheduler::getCurrentThread());
	}
	
};
