#include <Stream.h>
#include <Memory.h>
#include <lib.h>
namespace StreamManager
{
	Stream *CreateStream(uint32_t len)
	{
		Stream *out = (Stream *)malloc(sizeof(Stream));
		out->rindex = 0;
		out->windex = 0;
		out->buffer = (short *)malloc(len * 2);
		out->size = len;
		for (uint32_t i = 0; i < len; i++)
			out->buffer[i] = -80;
		return out;
	}
	void DestoryStream(Stream *stream)
	{
		free((char *)stream->buffer);
		free((char *)stream);
	}
};