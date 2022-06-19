#include <CGA.h>
#include <IO.h>
#include <Memory.h>
int CGA::sx = 0;
int CGA::sy = 0;
char CGA::screenColor = 0x0f;
bool lockedC = 0;
namespace CGA
{
	void printChar(char c, int x, int y)
	{
		((char *)0xb8000)[x * 2 + (y * 160)] = c;
		((char *)0xb8000)[x * 2 + (y * 160) + 1] = CGA::screenColor;
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
		if (sy >= 23)
		{
			memcpy((char *)0xB8000, (char *)0xB8000 + 160, 25 * 160);
			sy--;
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
			if (sy >= 24)
			{
				memcpy((char *)0xB8000, (char *)0xB8000 + 160, 25 * 160);
				sy--;
			}
		}
	}

	void clearScreen()
	{
		for (int i = 0; i < 160 * 25; i += 2)
		{
			((char *)0xB8000)[i] = 0;
		}
		for (int i = 1; i < 160 * 25; i += 2)
		{
			((char *)0xB8000)[i] = 0x0f;
		}
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
		printChar(0, sx, sy);
		if (sx < 80)
			CGA::setCursorPosition(sx, sy);
		else
			CGA::setCursorPosition(0, sy);
	}
};