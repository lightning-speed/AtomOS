#include <Mouse.h>
#include <Serial.h>
#include <IO.h>
#include <FB.h>
#include <Scheduler.h>
#define MOUSE_OUT 0xfffffffe
#define MOUSE_IN 0
using namespace IO;
#define mouse_size 7
#define MOUSE_ENABLE_PACKET 0xF4
#define MOUSE_DISABLE_PACKET 0xF5
#define MOUSE_USE_DEFAULT_SETTINGS 0xF6

/// Mouse ISR cycle.
uint8_t offset = 0;
/// Mouse communication data.
int8_t buffer[3];
int32_t Mouse::mouseX = 100;
int32_t Mouse::mouseY = 100;
char buttons;
int32_t px = 100, py = 100;
int32_t pbcolor[100];
//Some day I will move this to a file(I am too lazy)
uint32_t mouse[] = {1, 0, 0, 0, 0, 0, 0,
										1, 1, 0, 0, 0, 0, 0,
										1, 2, 1, 1, 0, 0, 0,
										1, 2, 2, 2, 1, 1, 0,
										1, 2, 2, 2, 1, 1, 0,
										1, 2, 1, 1, 0, 0, 0,
										1, 1, 0, 0, 0, 0, 0,
										1, 0, 0, 0, 0, 0, 0};

namespace Mouse
{
	void mouseUp()
	{
	}
	void mouseDown()
	{
		Stream *mouseStream = Scheduler::activeProcess()->mouseStream;
		mouseStream->push(mouseX);
		mouseStream->push(mouseY);
	}
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
				offset = 41;
				paintMouse();
				offset = 0;

				/*handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));*/
			}

			for (uint8_t i = 0; i < 3; i++)
			{
				if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i)))
				{
					if (buttons & (0x1 << i))
						mouseUp();
					else
						mouseDown();
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
		//Copy the square as it will be further used when mouse will be moved and the part will be redrawn
	}
	void paintMouse()
	{
		//IF SOMETHING WAS DRAWN OVER THE MOUSE
		if (MOUSE_OUT != FB::getPixel(px, py) || MOUSE_IN != FB::getPixel(px + 1, py))
		{
			//CHECK FOR THE PIXEL, IF CHNAGED RECORD IT
			for (int i = 0; i < mouse_size; i++)
				for (int j = 0; j < mouse_size; j++)
				{
					uint32_t color = 1;
					if (mouse[(i * mouse_size) + j] == 1)
						color = MOUSE_OUT;
					else if (mouse[(i * mouse_size) + j] == 2)
						color = MOUSE_IN;
					if (color != 1)
						if (FB::getPixel(px + j, py + i) != color)
							pbcolor[(i * mouse_size) + j] = FB::getPixel(px + j, py + i);
				}

			//RE DRAW MOUSE

			for (int i = 0; i < mouse_size; i++)
				for (int j = 0; j < mouse_size; j++)
					if (mouse[(i * mouse_size) + j] == 1)
						FB::setPixel(mouseX + j, mouseY + i, MOUSE_OUT);
					else if (mouse[(i * mouse_size) + j] == 2)
						FB::setPixel(mouseX + j, mouseY + i, MOUSE_IN);
		}

		//IF CALLED FROM THE INTERRUPT

		if (offset == 41)
		{

			mouseX += (int8_t)buffer[1];
			mouseY += -((int8_t)buffer[2]);
			if (mouseX < 3)
				mouseX = 3;
			else if (mouseX > FB::width - 10)
				mouseX = FB::width - 10;
			if (mouseY < 3)
				mouseY = 3;
			else if (mouseY > FB::height - 10)
				mouseY = FB::height - 10;

			//REPLACE THE MOUSE WITH ORIGINAL PIXELS

			for (int i = 0; i < mouse_size; i++)
				for (int j = 0; j < mouse_size; j++)
					FB::setPixel(px + j, py + i, pbcolor[(i * mouse_size) + j]);

			//RECORD THE ORIGRNAL PIXELS

			for (int i = 0; i < mouse_size; i++)
				for (int j = 0; j < mouse_size; j++)
					pbcolor[(i * mouse_size) + j] = FB::getPixel(mouseX + j, mouseY + i);

			//DRAW MOUSE

			for (int i = 0; i < mouse_size; i++)
				for (int j = 0; j < mouse_size; j++)
					if (mouse[(i * mouse_size) + j] == 1)
						FB::setPixel(mouseX + j, mouseY + i, MOUSE_OUT);
					else if (mouse[(i * mouse_size) + j] == 2)
						FB::setPixel(mouseX + j, mouseY + i, MOUSE_IN);
		}

		px = mouseX;
		py = mouseY;
	}

};