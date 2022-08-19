#include "atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)

typedef struct
{                              // Total: 54 bytes
    uint16_t type;             // Magic identifier: 0x4d42
    uint32_t size;             // File size in bytes
    uint16_t reserved1;        // Not used
    uint16_t reserved2;        // Not used
    uint32_t offset;           // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t dib_header_size;  // DIB Header size in bytes (40 bytes)
    int32_t width_px;          // Width of the image
    int32_t height_px;         // Height of image
    uint16_t num_planes;       // Number of color planes
    uint16_t bits_per_pixel;   // Bits per pixel
    uint32_t compression;      // Compression type
    uint32_t image_size_bytes; // Image size in bytes
    int32_t x_resolution_ppm;  // Pixels per meter
    int32_t y_resolution_ppm;  // Pixels per meter
    uint32_t num_colors;       // Number of colors
    uint32_t important_colors; // Important colors
} BMPHeader;

#pragma pack(pop)
uint8_t *data;
Window win;
void exitS()
{
    free(data);
    destoryWindow(win);
    set_screen(6, 0);
    exit(0);
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage imv [file path]");
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "File doesn't exist");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    BMPHeader *header;
    //max 1000x1000px
    data = malloc(size);
    fread(data, size, 1, f);
    fclose(f);
    header = (BMPHeader *)data;
    char title[64];
    memset(title, 0, 64);
    strcat((char *)&title, "Image Viewer: ");
    strcat((char *)&title, argv[1]);
    win = createWindow((char *)&title);
    setOnClose(win, (void *)exitS);
    Component back = createRect(0, 0, win->width, win->height, toColor(30, 30, 30));

    int ratio = 2;
    int x = (win->width - (header->width_px / ratio)) / 2;
    int y = (win->height - (header->height_px / ratio)) / 2;

    for (;;)
    {
        sleep(100);
        window_draw(win);
        window_update(win);
        int padding = header->width_px % 4;

        uint8_t *datat = (data + size) - 1;
        for (int i = 0; i < header->height_px; i++)
        {
            datat -= padding;
            for (int j = header->width_px - 1; j > -1; j--)
            {
                datat -= 3;
                if (j % ratio == 0 && i % ratio == 0)
                    setpixel(win, x + (j / ratio), y + (i / ratio), (datat[0] << 16) | (datat[2] << 8) | datat[1]);
            }
        }
        back->type = 100;
        repaint(win);
    }
}