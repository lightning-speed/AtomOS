#include <CGA.h>
#include <IO.h>
#include <FB.h>
#include <Memory.h>
int CGA::sx = 0;
int CGA::sy = 0;
char CGA::screenColor = 0x0f;
bool lockedC = 0;
int colors[] = {
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (255 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((255 << 24) | (60 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (0 << 16) | (0 << 8) | 255),
		((0 << 24) | (255 << 16) | (100 << 8) | 100),
		((255 << 24) | (250 << 16) | (40 << 8) | 170),
		((255 << 24) | (255 << 16) | (255 << 8) | 0),
		((255 << 24) | (255 << 16) | (255 << 8) | 255)};
namespace CGA
{
	void printChar(char c, int x, int y)
	{

		FB::drawTerminalAsciiChar(x, y, c, colors[CGA::screenColor]);
		if (x < 80)
			CGA::setCursorPosition(x + 1, y);
		else
			CGA::setCursorPosition(0, y + 1);
	}
	void printChar(char c)
	{
		if (c == '\t')
		{
			for (int j = 0; j < 4; j++)
				CGA::printChar(' ');
			return;
		}
		if (c != '\n')
		{
			CGA::printChar(c, sx, sy);
			sx++;
		}
		if (sx >= 80 || c == '\n')
		{
			sx = 0;
			sy++;
			if (sx < 80)
				CGA::setCursorPosition(sx, sy);
			else
				CGA::setCursorPosition(0, sy + 1);
		}
		if (sy >= 26)
		{
			clearScreen();
		}
	}
	void print(String s, char color)
	{
		CGA::screenColor = color;
		CGA::print(s);
		CGA::screenColor = 0x0f;
	}
	void print(String s)
	{
		for (uint32_t i = 0; i < s.length(); i++)
		{

			if (s.buffer[i] == '\t')
			{
				for (int j = 0; j < 4; j++)
					CGA::printChar(' ');
				i++;
			}
			if (s.buffer[i] != '\n')
			{
				CGA::printChar(s.buffer[i], sx, sy);
				sx++;
			}
			if (sx >= 80 || s.buffer[i] == '\n')
			{
				sx = 0;
				sy++;
				if (sx < 80)
					CGA::setCursorPosition(sx, sy);
				else
					CGA::setCursorPosition(0, sy + 1);
			}
			if (sy >= 26)
			{
				clearScreen();
			}
		}
	}

	void clearScreen()
	{
		FB::clearScreen();

		sx = 0;
		sy = 0;
	}
	void setCursorPosition(int xe, int ye)
	{
		unsigned temp;
		temp = ye * 80 + xe;
		IO::outb(0x3D4, 14);
		IO::outb(0x3D5, temp >> 8);
		IO::outb(0x3D4, 15);
		IO::outb(0x3D5, temp);
	}
	void backSpace()
	{
		if (sx != 0)
		{
			sx--;
		}
		else
		{
			sy--;
			sx = 80;
		}
		printChar(' ', sx, sy);
		if (sx < 80)
			CGA::setCursorPosition(sx, sy);
		else
			CGA::setCursorPosition(0, sy);
	}
};