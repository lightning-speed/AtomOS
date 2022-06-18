#include <KeyboardHandler.h>
#include <Memory.h>
#include <CGA.h>
KeyboardHandler::KeyboardHandler()
{
	buffer = (int *)malloc(500);
	index = 1;
}
int KeyboardHandler::fetch()
{
	if (index > 1)
	{
		index--;
		int out = buffer[index];
		for (int i = 0; i < index + 1; i++)
		{
			buffer[i] = buffer[i + 1];
		}
		return out;
	}
	return 0;
}
