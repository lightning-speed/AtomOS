#include <stdint.h>

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
    char *name;
    uint32_t *buffer;
} window_t;

typedef window_t *Window;