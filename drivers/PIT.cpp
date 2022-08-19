#include <PIT.h>
#include <CGA.h>
#include <IO.h>
#include <Scheduler.h>
#include <lib.h>
uint64_t PIT::timeSinceBoot = 0;
uint16_t PIT::divisor = 0;
using namespace IO;
namespace PIT
{
	void init()
	{
		set_interrupt_handler(0, (void *)handler);
		setDivisor(PIT_DIVISOR);
	}

	void handler(register_t *regs)
	{
		uint64_t timePassed = TIME_CHANGE_PER_CALL;
		Scheduler::isScheduling = true;
		Scheduler::schedule(regs, timePassed);
		Scheduler::isScheduling = false;

		timeSinceBoot += timePassed;
	}

	void setDivisor(uint16_t d)
	{
		if (d < 100)
			d = 100;
		divisor = d;
		outb(0x40, (uint8_t)(d & 0x00ff));
		asm("nop");
		outb(0x40, (uint8_t)((d & 0xff00) >> 8));
	}
	void setFrequency(uint64_t frequency)
	{
		PIT::setDivisor(div(PIT_CLOCKING_FREQUENCY, frequency));
	}
	void sleep(uint32_t milliseconds)
	{
		uint64_t pre = PIT::timeSinceBoot + (milliseconds);
		while (pre > PIT::timeSinceBoot)
		{
			asm("hlt");
		}
	}
	void sync(int timetaken)
	{
		TIME_CHANGE_PER_CALL = (int)(((double)1000 / (double)timetaken) * 1000) / PIT_FREQUENCY;
	}
}