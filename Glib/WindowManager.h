#include "Window.h"
#include <String.h>
namespace WindowManager
{
    Window create(String name, uint16_t width, uint16_t height, int mode);
    Window create(String name);
    void drawChar(Window w, uint64_t x, uint64_t y, char c, int color);
    void drawTerminalAsciiChar(Window w, int x, int y, char c, int color);
    static void repaint(Window w);
    void clear(Window w);
    void destroy(Window w);
};