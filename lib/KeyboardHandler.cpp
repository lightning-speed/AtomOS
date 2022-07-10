#include <KeyboardHandler.h>
#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
namespace KeyboardManager
{
	KeyboardHandler create()
	{
		KeyboardHandler handler = (KeyboardHandler)malloc(sizeof(KeyboardHandler_t));
		handler->buffer = (uint16_t *)malloc(500);
		handler->index = 1;
		return handler;
	}
	int fetch(KeyboardHandler handler)
	{
		if (handler->index > 1)
		{
			handler->index--;
			int out = handler->buffer[handler->index];
			for (int i = 0; i < handler->index + 1; i++)
			{
				handler->buffer[i] = handler->buffer[i + 1];
			}
			return out;
		}
		return 0;
	}
	void del(KeyboardHandler handler)
	{
		free((char *)handler->buffer);
		free((char *)handler);
		Serial::log("kh destroyed");
	}
};