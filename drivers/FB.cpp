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
	void setPixel(int x, int y, int color)
	{
		uint32_t where = (x * 4 + y * pitch) / 4;
		((uint32_t *)addr)[where] = color;
	}
	void setPixel(char *buff, int x, int y, int color)
	{
		uint32_t where = (x * 4 + y * pitch) / 4;
		((uint32_t *)addr)[where] = color;
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
					setPixel(x + j, i + y, backColor);
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
	void repaint(char *buff, int x, int y, int tox, int toy)
	{
		memcpy(addr + ((x * 4 + y * pitch)), buff + ((x * 4 + y * pitch)), ((tox * 4 + toy * pitch)));
	}
}