#include "Sys.h"
#include <CGA.h>
#include <Scheduler.h>
#include <Keyboard.h>
#include <FS.h>
#include <Runtime.h>
#include <Memory.h>
#include <CMOS.h>
#include <VFS.h>
#include "../Glib/WindowManager.h"
#include <Serial.h>
#include <FB.h>

//some syscalls are not completed yet or are in test mode and hence have bad code
namespace Sys
{
	void exit(register_t *regs)
	{
		if (Scheduler::getCurrentThread()->parent != nullptr)
			CGA::print("\nProcess [" + ((process_t *)(Scheduler::getCurrentThread()->parent))->name + "]: Killed with exit code " + (int)regs->ebx + "\n", 0x0d);
		else
			CGA::print((String) "\nProcess [unknown]: Killed with exit code " + (int)regs->ebx + "\n", 0x0d);
		Runtime::kill((process_t *)Scheduler::getCurrentThread()->parent);
	}
	void write(register_t *regs)
	{
		uint32_t stream = regs->ebx;
		char *data = (char *)regs->ecx;
		uint32_t len = regs->edx;
		process_t *proc = ((process_t *)Scheduler::getCurrentThread()->parent);
		switch (stream)
		{
		case 0:
			if (len != 0)
				for (uint32_t i = 0; i < len; i++)
				{
					CGA::printChar((char)data[i]);
				}

			else
			{
				CGA::printChar((char)((int)data));
			}
			break;
		case 2:
			CGA::screenColor = 0x0c;
			if (len != 0)
				for (uint32_t i = 0; i < len; i++)
					CGA::print((char)data[i]);
			else
			{
				CGA::printChar((char)((int)data));
			}
			CGA::screenColor = 0x0f;
			break;
		default:
			if (len == 0)
				VFS::write((fnode *)stream, (char)((int)data));
			else
				for (uint32_t i = 0; i < len; i++)
					VFS::write((fnode *)stream, (char)data[i]);
			break;
		}
	}
	void read(register_t *regs)
	{
		uint32_t stream = regs->ebx;
		switch (stream)
		{
		case 1:
			regs->ecx = ((process_t *)Scheduler::getCurrentThread()->parent)->keyboardStream->fetch();
			if (regs->ecx != '\b' && regs->ecx != 0)
				CGA::printChar(regs->ecx);
			break;

		default:
			if (regs->edx == 0)
				regs->ecx = VFS::read((fnode *)stream, regs->ecx);
			else
			{
				VFS::read((char *)regs->ecx, regs->edx, (fnode *)regs->ebx);
			}
			break;
		}
	}
	void open(register_t *regs)
	{
		char *name = (char *)regs->ebx;
		char *mode = (char *)regs->ecx;
		regs->ecx = (uint32_t)(VFS::open(name, mode));
	}
	void close(register_t *regs)
	{
		fnode *file = (fnode *)regs->ebx;
		VFS::close(file);
	}
	void exec(register_t *regs)
	{
		char **args = (char **)regs->ecx;
		process_t *proc = Runtime::exec(args[0], regs->ebx, (char **)args, (char **)regs->edx);
		if (proc != nullptr)
		{
			regs->ecx = (uint32_t)proc->children[0];
		}
		else
		{
			proc = Runtime::exec((String)args[0] + ".exe", regs->ebx, (char **)args, (char **)regs->edx);
			if (proc != nullptr)
				regs->ecx = (uint32_t)proc->children[0];
			else
				regs->ecx = (uint32_t) nullptr;
		}
	}
	void sleep(register_t *regs)
	{
		Scheduler::sleepThread(Scheduler::getCurrentThread(), regs->ebx);
	}
	void alloc(register_t *regs)
	{

		switch (regs->ebx)
		{
		case 0:
			regs->ecx = (uint32_t)malloc(regs->ecx);
			break;
		case 1:
			regs->ecx = (uint32_t)realloc((char *)regs->ecx, regs->edx);
			break;
		case 2:
			free((char *)regs->ecx);
			break;
		default:
			break;
		}
	}
	void screenset(register_t *regs)
	{
		switch (regs->ebx)
		{
		case 0:
			CGA::screenColor = regs->ecx;
			break;
		case 1:
			CGA::sx = regs->ecx;
			break;
		case 2:
			CGA::sy = regs->ecx;
			break;
		case 3:
			CGA::sx -= regs->ecx;
			break;
		case 4:
			CGA::sy -= regs->ecx;
			break;
		case 5:
			CGA::backSpace();
			break;
		case 6:
			CGA::clearScreen();
		default:
			break;
		}
	}
	void time(register_t *regs)
	{
		switch (regs->ebx)
		{
		case 0:
			regs->ecx = CMOS::getSeconds();
			break;
		case 1:
			regs->ecx = CMOS::getMinutes();
			break;
		case 2:
			regs->ecx = CMOS::getHours();
			break;
		case 3:
			regs->ecx = CMOS::getDays();
			break;
		case 4:
			regs->ecx = CMOS::getMonths();
			break;
		case 5:
			regs->ecx = CMOS::getYears();
			break;
		default:
			break;
		}
	}
	void env(register_t *regs)
	{
		switch (regs->ebx)
		{
		case 0:
			regs->ecx = (uint32_t)(((process_t *)Scheduler::getCurrentThread()->parent)->data->env);
		}
	}
	void rem(register_t *regs)
	{
		fnode *f = VFS::open((char *)regs->ebx, "r");
		if (f != nullptr)
		{
			regs->ecx = VFS::del(f);
		}
		else
			regs->ecx = 0;
	}
	void wm(register_t *regs)
	{
		uint16_t *arrg;
		switch (regs->ebx)
		{
		case 0:
			regs->ecx = (uint32_t)WindowManager::create((String)(char *)regs->ecx, 0);
			break;
		case 1:
			regs->ecx = (uint32_t)FB::addr;
			break;
		case 2:
			regs->ecx = (uint32_t)FB::width;
			break;

		case 3:
			regs->ecx = (uint32_t)FB::height;
			break;
		case 4:
			WindowManager::destroy((Window)regs->ecx);
			break;
		case 5:
			arrg = (uint16_t *)regs->ecx;
			FB::drawCharTransparent(arrg[0], arrg[1], arrg[2], regs->edx);
			break;
		default:
			break;
		}
	}
	void pg(register_t *regs)
	{
		thread_t *main_thread = (thread_t *)regs->ebx;
		process_t *parent = (process_t *)main_thread->parent;
		switch (regs->ecx)
		{
		case 0:
			break;

		default:
			break;
		}
	}

};