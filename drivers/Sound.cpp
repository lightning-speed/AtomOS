#include <Sound.h>
#include <PIT.h>
#include <Scheduler.h>
#include <IO.h>
namespace Sound
{
	//Play sound using built in speaker
	void play_sound(uint32_t nFrequence)
	{
		uint32_t Div;
		uint8_t tmp;

		Div = 1193180 / nFrequence;
		IO::outb(0x43, 0xb6);
		IO::outb(0x42, (uint8_t)(Div));
		IO::outb(0x42, (uint8_t)(Div >> 8));

		tmp = IO::inb(0x61);
		if (tmp != (tmp | 3))
		{
			IO::outb(0x61, tmp | 3);
		}
	}

	void nosound()
	{
		uint8_t tmp = IO::inb(0x61) & 0xFC;
		IO::outb(0x61, tmp);
	}

	void play(uint32_t freq, uint32_t time)
	{
		play_sound(freq);
		asm("hlt");
		nosound();
	}
};