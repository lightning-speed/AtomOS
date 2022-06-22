#pragma once
#include <stdint.h>
#include <VFS.h>

namespace FB
{
	extern int width, height, pitch;
	extern char *addr;
	extern char *font;

	void init(char *addr, int width, int height, int pitch);
	void setPixel(int x, int y, int color);
	void drawChar(uint64_t x, uint64_t y, char c, int color);
	void loadFont(fnode *f);
	void drawTerminalAsciiChar(int x, int y, char c, int color);
	void scroll();
	void clearScreen();
}