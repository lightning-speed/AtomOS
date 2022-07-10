#pragma once
#include <stdint.h>

typedef struct
{
	uint16_t *buffer;
	int index;
} KeyboardHandler_t;

typedef KeyboardHandler_t *KeyboardHandler;

namespace KeyboardManager
{
	KeyboardHandler create();
	int fetch(KeyboardHandler handler);
	void del(KeyboardHandler handler);
};
