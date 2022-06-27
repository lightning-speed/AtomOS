#include "./WindowManager.h"
#include <FB.h>
#include <Memory.h>
extern uint32_t backColor;
namespace WindowManager
{
    Window create(String name, uint16_t width, uint16_t height, int mode)
    {
        Window out = (Window)malloc(sizeof(window_t));
        out->name = name.buffer;
        out->width = width;
        out->height = height;
        out->x = 0;
        out->y = 0;
        if (mode == 0)
            out->buffer = (uint32_t *)FB::addr;
        else
            out->buffer = (uint32_t *)malloc(width * height * 4);
        return out;
    }
    void drawChar(Window w, uint64_t x, uint64_t y, char c, int color)
    {
        for (uint64_t i = 0; i < 16; i++)
        {
            int r = ((i + y) * FB::width);
            for (uint64_t j = 0; j < 8; j++)
            {
                if ((FB::font[(c * 16) + i] & (0x80 >> j)) != 0)
                {
                    ((uint32_t *)w->buffer)[r + (x + j)] = color;
                }
                else
                    ((uint32_t *)w->buffer)[r + (x + j)] = backColor;
            }
        }
    }
    void drawTerminalAsciiChar(Window w, int x, int y, char c, int color)
    {
        if (c == 0)
            c = ' ';
        int xx = x * 9;
        int yy = y * 18;
        drawChar(w, xx, yy, c, color);
    }
    void repaint(Window w, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
    {
        FB::repaint((char *)w->buffer, sx, sy, sx + width, sy + height);
    }
    void repaint(Window w)
    {
        repaint(w, 0, 0, w->width, w->height);
    }
};