#include <FB.h>
#include <Memory.h>

int FB::height;
int FB::width;
int FB::pitch;
char *FB::addr;
char *FB::font;
namespace FB
{
	void init(char *addr, int width, int height, int pitch)
	{
		FB::width = width;
		FB::height = height;
		FB::addr = addr;
		FB::pitch = pitch;
	}
	void loadFont(fnode *f)
	{
		font = f->content;
	}
	void setPixel(int x, int y, int color)
	{
		unsigned where = x * 4 + y * pitch;
		addr[where] = color & 255;						 // BLUE
		addr[where + 1] = (color >> 8) & 255;	 // GREEN
		addr[where + 2] = (color >> 16) & 255; // RED
	}
	void drawChar(uint64_t x, uint64_t y, char c, int color)
	{
		for (uint64_t i = 0; i < 16; i++)
		{
			for (uint64_t j = 0; j < 8; j++)
			{
				if ((font[(c * 16) + i] & (0x80 >> j)) != 0)
				{
					setPixel(x + j, i + y, color);
				}
				else
					setPixel(x + j, i + y, 1);
			}
		}
	}
	void drawTerminalAsciiChar(int x, int y, char c, int color)
	{
		if (c == 0)
			c = ' ';
		int xx = x * 9;
		int yy = y * 18;
		drawChar(xx, yy, c, color);
	}
	void scroll()
	{
		memcpy(addr, addr + (width * 4 + 18 * pitch), (width * 4 + 18 * pitch) * 24);
	}
	void clearScreen()
	{
		memset(addr, 0, width * 4 + height * pitch);
	}
}