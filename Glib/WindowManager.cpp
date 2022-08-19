#include "./WindowManager.h"
#include <FB.h>
#include <Memory.h>
#include <Serial.h>
#include <lib.h>
extern uint32_t backColor;
namespace WindowManager
{
	static inline void repaint(Window win)
	{
		if (win == nullptr)
			return;
		uint32_t r = win->width * win->height;
		for (uint32_t i = 0; i < r; i++)
		{
			((uint32_t *)FB::addr)[i] = (win->buffer)[i];
		}
	}
	Window create(string name)
	{
		return create(name, 0);
	}
	Window create(string name, int mode)
	{
		Window out = (Window)malloc(sizeof(window_t));
		out->name = (char *)kernel_clone(name.buffer, name.length());
		out->width = FB::width;
		out->height = FB::height;
		out->x = 0;
		out->y = 0;
		if (mode == 0)
			out->buffer = (uint32_t *)FB::addr;
		else
			out->buffer = (uint32_t *)malloc(FB::width * FB::height * 4);
		Serial::log("Window Created [ " + name + " ]\n");
		return out;
	}
	void drawCharBlock(Window w, uint64_t x, uint64_t y, char c, uint32_t color)
	{
		if (w == nullptr)
			return;

		for (uint64_t i = 0; i < 16; i++)
		{
			int r = ((i + y) * FB::width);
			char f = FB::font[(c * 16) + i];
			for (uint64_t j = 0; j < 8; j++)
			{
				if ((f & (0x80 >> j)) != 0)
				{
					((uint32_t *)w->buffer)[r + (x + j)] = color;
				}
				else
					((uint32_t *)w->buffer)[r + (x + j)] = backColor;
			}
		}
	}
	void drawChar(Window w, uint64_t x, uint64_t y, char c, uint32_t color)
	{
		if (w == nullptr)
			return;

		for (uint64_t i = 0; i < 16; i++)
		{
			int r = ((i + y) * FB::width);
			char f = FB::font[(c * 16) + i];
			for (uint64_t j = 0; j < 8; j++)
			{
				if ((f & (0x80 >> j)) != 0)
				{
					((uint32_t *)w->buffer)[r + (x + j)] = color;
				}
			}
		}
	}
	void drawTerminalAsciiChar(Window w, int x, int y, char c, int color)
	{
		if (w == nullptr)
			return;

		if (c == 0)
			c = ' ';
		int xx = x * 9;
		int yy = y * 18;
		drawCharBlock(w, xx, yy, c, color);
	}

	void clear(Window w)
	{
		if (w == nullptr)
			return;
		int r = w->width * w->height;
		for (int i = 0; i < r; i++)
		{
			w->buffer[i] = backColor;
		}
	}
	void destroy(Window w)
	{
		if (w == nullptr)
			return;
		free((char *)w->buffer);
		free(w->name);
		free((char *)w);
		Serial::log("Window Destroyed [ " + (string)w->name + " ]\n");
	}
};