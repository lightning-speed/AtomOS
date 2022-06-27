#include <stdint.h>
#include <IDT.h>

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_IRQ 12

#define MOUSE_PORT 0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT 0x02
#define MOUSE_BBIT 0x01
#define MOUSE_WRITE 0xD4
#define MOUSE_F_BIT 0x20
#define MOUSE_V_BIT 0x08

typedef struct
{
    uint32_t magic;
    int8_t x_difference;
    int8_t y_difference;
    uint8_t buttons;
} mouse_device_packet_t;

#define MOUSE_MAGIC 0xFEED1234

namespace Mouse
{
    void init();
    void handler(register_t *regs);
    void mouse_wait(uint8_t a_type);
    uint8_t mouse_read();
    void mouse_write(uint8_t write);
};