#include <Keyboard.h>
#include <CGA.h>
#include <String.h>
#include <IO.h>
#include <IDT.h>
#include <Scheduler.h>
#include <KeyboardHandler.h>
#include "KeyboardScancodes.h"
uint16_t ScanCode;
bool CapsOn = 0;
bool ShiftPressed = 0;
bool CtrlPressed = 0;
int Keyboard::CharPressed = 0;
extern process_t *kernel_stage2proc;
namespace Keyboard
{
	void init()
	{
		set_interrupt_handler(1, (void *)Keyboard::handler);
	}
	void handler(register_t *regs)
	{
		ScanCode = IO::inb(0x60);
		if (ScanCode == 0x2A)
			ShiftPressed = 1;
		else if (ScanCode == 0xAA)
			ShiftPressed = 0;
		else if (ScanCode == 0x1D)
			CtrlPressed = 1;
		else if (ScanCode == 0x9D)
			CtrlPressed = 0;
		else if (ScanCode == 0x3A)
			CapsOn = !CapsOn;
		else if (ScanCode < 128)
		{
			int c;
			if (ShiftPressed)
				c = code_shfit[ScanCode];
			else if (CapsOn)
				c = code_caps[ScanCode];
			else
				c = codes[ScanCode];
			Keyboard::CharPressed = c;

			process_t *proc = (process_t *)(Scheduler::processes[Scheduler::processCount - 1]);
			//THIS IS A TEMPERORY CODE AND IS JUST FOR TEST
			if ((c == 'c' || c == 'C') && CtrlPressed == true)
			{
				if ((proc->name == "cmd.exe") == 0)
				{
					Scheduler::killProcess(proc);
					CGA::print("^C\n");
				}
			}

			if (proc != nullptr)
			{
				proc->keyboardHandler->buffer[proc->keyboardHandler->index] = c;
				proc->keyboardHandler->index++;
			}
		}
	}
}