#include <RMm.h>
#include <Serial.h>
#define MAX_HEAP_BLOCKS 50
#define HEAP_BLOCK_SIZE 0x500000
#define HEAP_BASE 0x1900000
HeapBlock blocks[MAX_HEAP_BLOCKS];
bool RMm::initalized = false;
namespace RMm
{
    void init()
    {
        for (int i = 0; i < MAX_HEAP_BLOCKS; i++)
        {
            blocks[i].start = (char *)(HEAP_BASE + (HEAP_BLOCK_SIZE * i));
            blocks[i].size = HEAP_BLOCK_SIZE;
            blocks[i].inUse = false;
        }
        initalized = true;
    }
    HeapBlock *request(process_t *proc)
    {
        if (!RMm::initalized)
            RMm::init();
        if (proc == nullptr)
            return nullptr;
        HeapBlock *out = (HeapBlock *)malloc(sizeof(HeapBlock));
        for (int i = 0; i < MAX_HEAP_BLOCKS; i++)
        {
            if (!blocks[i].inUse)
            {
                out = (HeapBlock *)&blocks[i];
                break;
            }
        }

        out->user = proc;
        out->inUse = true;

        return out;
    }
    void free(process_t *proc)
    {
        if (!RMm::initalized)
            RMm::init();
        if (proc == nullptr)
        {

            return;
        }

        for (int i = 0; i < MAX_HEAP_BLOCKS; i++)
            if (blocks[i].user == proc)
            {
                blocks[i].inUse = false;
                break;
            }
    }
};