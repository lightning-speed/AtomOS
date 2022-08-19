#include <stdint.h>
extern "C" uint64_t div(uint64_t num, uint32_t div);

void *kernel_clone(void *data, size_t size);