#include <FB.h>

int FB::height;
int FB::width;
int FB::pitch;
char *FB::addr;
namespace FB
{
	void init(char *addr, int width, int height, int pitch)
	{
		FB::width = width;
		FB::height = height;
		FB::addr = addr;
		FB::pitch = pitch;
	}
	void setPixel(int x, int y, int color)
	{
		unsigned where = x * 4 + y * pitch;
		addr[where] = color & 255;						 // BLUE
		addr[where + 1] = (color >> 8) & 255;	 // GREEN
		addr[where + 2] = (color >> 16) & 255; // RED
	}
}