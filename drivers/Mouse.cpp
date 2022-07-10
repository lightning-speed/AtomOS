#include <Mouse.h>
#include <Serial.h>
#include <IO.h>
#include <FB.h>

using namespace IO;

#define MOUSE_ENABLE_PACKET 0xF4
#define MOUSE_DISABLE_PACKET 0xF5
#define MOUSE_USE_DEFAULT_SETTINGS 0xF6

/// Mouse ISR cycle.
uint8_t offset = 0;
/// Mouse communication data.
int8_t buffer[3];
int32_t mouseX = 100;
int32_t mouseY = 100;
char buttons;

namespace Mouse
{
    static void wait(unsigned char type)
    {
        register unsigned int _time_out = 100000;
        if (type == 0)
        {
            // DATA.
            while (_time_out--)
            {
                if ((inb(0x64) & 1) == 1)
                {
                    return;
                }
            }
            return;
        }
        else
        {
            while (_time_out--) // SIGNALS
            {
                if ((inb(0x64) & 2) == 0)
                {
                    return;
                }
            }
            return;
        }
    }

    static void write(unsigned char data)
    {
        wait(1);
        outb(0x64, 0xD4);
        wait(1);
        outb(0x60, data);
    }

    static unsigned char read()
    {
        wait(0);
        return inb(0x60);
    }

    void handler(register_t *regs)
    {
        uint8_t status = inb(0x64);
        if (!(status & 0x20))
            return;

        buffer[offset] = inb(0x60);

        offset = (offset + 1) % 3;

        if (offset == 0)
        {
            if (buffer[1] != 0 || buffer[2] != 0)
            {
                FB::setPixel(mouseX, mouseY, 0xffffffff);
                mouseX += (int8_t)buffer[1];
                mouseY += -((int8_t)buffer[2]);
                /*handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));*/
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i)))
                {
                    /*if (buttons & (0x1 << i))
                        handler->OnMouseUp(i + 1);
                    else
                        handler->OnMouseDown(i + 1);*/
                }
            }
            buttons = buffer[0];
        }
    }
    static void mouse_enable()
    {
        // Enable the mouse interrupts.
        // Disable the mouse.
        write(MOUSE_ENABLE_PACKET);
        // Acknowledge.
        read();
    }

    void init()
    {
        // Enable the auxiliary mouse device.
        wait(1);
        outb(0x64, 0xA8);

        // Enable the interrupts.
        wait(1);
        outb(0x64, 0x20);
        wait(0);
        uint8_t status_byte = (inb(0x60) | 2);
        wait(1);
        outb(0x64, 0x60);
        wait(1);
        outb(0x60, status_byte);

        // Tell the mouse to use default settings.
        write(MOUSE_USE_DEFAULT_SETTINGS);
        // Acknowledge.
        read();

        // Setup the mouse handler.
        set_interrupt_handler(12, (void *)handler);

        // Enable the mouse.
        mouse_enable();
    }

};