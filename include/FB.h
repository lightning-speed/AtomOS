#pragma once
#include <stdint.h>

namespace FB
{
	extern int width, height, pitch;
	extern char *addr;
	void init(char *addr, int width, int height, int pitch);
	void setPixel(int x, int y, int color);
}