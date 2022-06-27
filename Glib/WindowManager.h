#include "Window.h"
#include <String.h>
namespace WindowManager
{
    Window create(String name, uint16_t width, uint16_t height, int mode);
    void drawChar(Window w, uint64_t x, uint64_t y, char c, int color);
    void drawTerminalAsciiChar(Window w, int x, int y, char c, int color);
    void repaint(Window w, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
    void repaint(Window w);
};