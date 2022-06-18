#include <stdint.h>
extern "C" uint64_t div(uint64_t n, uint32_t b)
{
	uint64_t a = n;
	uint32_t upper = ((uint32_t *)&a)[1];
	uint32_t lower = ((uint32_t *)&a)[0];
	upper /= b;
	lower /= b;
	uint64_t o = 0;
	((uint32_t *)&o)[1] = upper;
	((uint32_t *)&o)[0] = lower;
	return o;
}