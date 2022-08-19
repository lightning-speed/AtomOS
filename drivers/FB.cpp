#include <FB.h>
#include <Memory.h>

int FB::height;
int FB::width;
int FB::pitch;
char *FB::addr;
char *FB::font;
uint32_t backColor = ((255 << 24) | (46 << 16) | (02 << 8) | 79);
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

	void drawCharBlock(uint64_t x, uint64_t y, char c, int color)
	{
		for (uint64_t i = 0; i < 16; i++)
		{
			int r = ((i + y) * FB::width);
			char f = FB::font[(c * 16) + i];
			for (uint64_t j = 0; j < 8; j++)
			{
				if ((f & (0x80 >> j)) != 0)
				{
					((uint32_t *)FB::addr)[r + (x + j)] = color;
				}
				else
					((uint32_t *)FB::addr)[r + (x + j)] = backColor;
			}
		}
	}
	void drawChar(uint64_t x, uint64_t y, char c, int color)
	{
		for (uint64_t i = 0; i < 16; i++)
		{
			int r = ((i + y) * FB::width);
			char f = FB::font[(c * 16) + i];
			for (uint64_t j = 0; j < 8; j++)
			{
				if ((f & (0x80 >> j)) != 0)
				{
					((uint32_t *)FB::addr)[r + (x + j)] = color;
				}
			}
		}
	}
	void drawTerminalAsciiChar(int x, int y, char c, int color)
	{
		if (c == 0)
			c = ' ';
		int xx = x * 9;
		int yy = y * 18;
		drawCharBlock(xx, yy, c, color);
	}
	void scroll()
	{
		memcpy(addr, addr + (width * 18) * 4, (width * 14) * 25 * 4);
	}
	void clearScreen()
	{
		for (int i = 0; i < height; i++)
		{
			int r = i * FB::width;
			for (int j = 0; j < width; j++)
				((uint32_t *)addr)[r + j] = backColor;
		}
	}
	void repaint(char *buff)
	{
		memcpy(addr, buff, (width * 4 + height * pitch));
	}
	void repaint(uint32_t *buff, int x, int y, int tox, int toy)
	{
		for (int i = 0; i < height; i++)
		{
			int r = i * FB::width;
			for (int j = 0; j < width; j++)
				((uint32_t *)addr)[r + j] = ((uint32_t *)buff)[r + j];
		}
	}
}