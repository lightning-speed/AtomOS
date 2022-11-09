#include <Keyboard.h>
#include <CGA.h>
#include <String.h>
#include <IO.h>
#include <IDT.h>
#include <Scheduler.h>
#include "KeyboardScancodes.h"
#include <Runtime.h>
uint16_t ScanCode;
bool CapsOn = false;
bool ShiftPressed = false;
bool CtrlPressed = false;
int Keyboard::CharPressed = 0;
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
			ShiftPressed = true;
		else if (ScanCode == 0xAA)
			ShiftPressed = false;
		else if (ScanCode == 0x1D)
			CtrlPressed = true;
		else if (ScanCode == 0x9D)
			CtrlPressed = false;
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

			//process_t *proc = (process_t *)Scheduler::processes.end();
			//THIS IS A TEMPERORY CODE AND IS JUST FOR TEST
			if ((c == 'c' || c == 'C') && CtrlPressed == true)
			{				
				Runtime::fullTrace(((thread_t *)Scheduler::threads.end()),((thread_t *)Scheduler::threads.end())->regs);
			}

			
		}
	}
}