#pragma once

#include <stdint.h>
#include <FS.h>

class Ramdisk
{
private:
public:
	static void init();
	static char *start;
	static uint32_t size;
};