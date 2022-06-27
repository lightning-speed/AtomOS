#pragma once
#include <stdint.h>

class KeyboardHandler
{
private:
public:
	int *buffer;
	int index;
	int fetch();
	KeyboardHandler();
	void del();
};