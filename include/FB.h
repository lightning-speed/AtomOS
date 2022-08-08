#pragma once
#include <stdint.h>
#include <VFS.h>

namespace FB
{
	extern int width, height, pitch;
	extern char *addr;
	extern char *font;
	extern char *buff;
	void init(char *addr, int width, int height, int pitch);
	void setPixel(int x, int y, uint32_t color);
	void setPixel(char *buff, int x, int y, uint32_t color);
	uint32_t getPixel(int x, int y);
	void drawChar(uint64_t x, uint64_t y, char c, int color);
	void drawCharTransparent(uint64_t x, uint64_t y, char c, int color);
	void loadFont(fnode *f);
	void drawTerminalAsciiChar(int x, int y, char c, int color);
	void scroll();
	void clearScreen();
	void repaint(char *buff);
	void repaint(uint32_t *buff, int x, int y, int tox, int toy);
}