#include <Process.h>
#include <Scheduler.h>
#include <CGA.h>

List processes;
uint32_t lasPID = 0;
namespace ProcessManager{
	uint32_t allocPID(){
		lasPID++;
		return lasPID;
	}
	process_t * create(string name,int argc,char ** argv,ENV_VARS* env_vars,void * entry){
		if(entry==nullptr)
			return nullptr;
		process_t out;
		memcpy(out.name,name.buffer,name.length());
		out.argc = argc;
		out.argv = argv;
		memcpy((char *)&(out.env_vars),(char *)env_vars,sizeof(ENV_VARS));
		out.entry = entry;
		out.PID = allocPID();
		out.state = INITALIZED;
		process_t * f = (process_t *)kernel_clone((void *)&out,sizeof(process_t));
		processes.push(f);
		return f;
	}
	void start(process_t*proc){
		if(proc==nullptr)
			return;
		thread_t * thread  = Scheduler::createThread((void *)proc,nullptr,proc->entry);
		proc->state = RUNNING;
		proc->threads.push((void *)thread);
	}
	void addThread(process_t * proc,void * func){
		if(func!=nullptr&&proc!=nullptr)
			attachThread(proc,Scheduler::createThread(proc,nullptr,func));
	}
	void attachThread(process_t * proc,thread_t * thread){
		if(proc==nullptr||thread==nullptr)
			return;
		thread->parent = (void *)proc;
		proc->threads.push(thread);
	}
	process_t * fromPID(uint32_t pid){
		for(uint32_t i = 0;i<processes.length();i++){
			process_t * proc = (process_t *)processes.at(i);
			if(proc->PID==pid){
				return proc;
			}
		}
		return nullptr;
	}
	void kill(process_t * proc,int exit_no){
		if(proc==nullptr||proc->state == KILLED) return;
		processes.remove(proc);
		for(uint32_t i = 0;i<proc->threads.length();i++){
			thread_t * thread = (thread_t *)(proc->threads.at(i));
			if(thread==nullptr){
				CGA::print("Thread Found null while killing thread");
			}
			thread->state = KILL_REQUESTED;
			thread->locked = true;
		}
		proc->state = KILLED;
		proc->exit_no = exit_no;
		
	}
	void selfKill(int exit_no){
		process_t * proc = (process_t *)Scheduler::getCurrentThread()->parent;
		kill(proc,exit_no);
		if(!inInterrupt)
			__asm__ __volatile__("hlt");
	}

};