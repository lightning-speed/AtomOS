#include <BitmapImage.h>
#include <FB.h>

namespace BitmapImage
{
	void drawImage(int x, int y, char *id, uint32_t size)
	{
		BMPHeader *header = (BMPHeader *)id;
		uint8_t *data = (uint8_t *)(id + size - 1);
		int padding = header->width_px % 4;
		for (int i = 0; i < header->height_px; i++)
		{
			data -= padding;
			for (int j = header->width_px - 1; j > -1; j--)
			{
				data -= 3;
				FB::setPixel(x + j, y + i, (data[0] << 16) | (data[2] << 8) | data[1]);
			}
		}
	}
}