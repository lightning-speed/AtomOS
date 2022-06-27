#include <Mouse.h>
#include <Serial.h>
#include <IO.h>

using namespace IO;

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];

namespace Mouse
{
    void init()
    {
    }

    void handler(register_t *regs)
    {
    }
};