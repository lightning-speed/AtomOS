#pragma once
#include <IDT.h>
#include <PIT.h>
#define PIT_CLOCKING_FREQUENCY 1193182
#define PIT_FREQUENCY 300
#define PIT_DIVISOR PIT_CLOCKING_FREQUENCY / PIT_FREQUENCY
namespace PIT
{
	void handler(register_t *regs);
	void init();
	void setDivisor(uint16_t d);
	void setFrequency(uint64_t frequency);
	void sleep(uint32_t milliseconds);
	void sync(int timetaken);
	extern uint64_t timeSinceBoot;
	extern uint16_t divisor;
};
