
#include <stdint.h>
#include <Process.h>

typedef struct
{
    char *start;
    uint32_t size;
    process_t *user;
    bool inUse;
} HeapBlock;

namespace RMm
{
    extern bool initalized;
    void init();
    HeapBlock *request(process_t *proc);
    void free(process_t *proc);
};