#pragma once
#include <stdint.h>

namespace Sound
{
	void play_sound(uint32_t nFrequence);
	void nosound();
	void play(uint32_t freq, uint32_t time);
};