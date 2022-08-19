#include <IO.h>
namespace IO
{
    uint8_t inb(uint16_t _port)
    {
        uint8_t rv;
        __asm__ __volatile__("inb %1, %0"
                             : "=a"(rv)
                             : "dN"(_port));
        return rv;
    }

    void outb(uint16_t _port, uint8_t _data)
    {
        __asm__ __volatile__("outb %1, %0"
                             :
                             : "dN"(_port), "a"(_data));
    }
    void outw(uint16_t _port, uint16_t _data)
    {
        __asm__ __volatile__("outw %1, %0"
                             :
                             : "dN"(_port), "a"(_data));
    }
    uint16_t inw(uint16_t _port)
    {
        uint16_t rv;
        __asm__ __volatile__("inw %1, %0"
                             : "=a"(rv)
                             : "dN"(_port));
        return rv;
    }
    void io_wait()
    {
        asm volatile("outb %%al, $0x80"
                     :
                     : "a"(0));
    }
};