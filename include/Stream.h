#pragma once

#include <stdint.h>
typedef struct __attribute__((packed))
{
    short *buffer;
    uint32_t windex;
    uint32_t rindex;
    uint32_t size;
    int fetch()
    {
        int out = this->buffer[this->rindex];
        if (out == -80)
            return 0;
        this->buffer[this->rindex] = -80;
        this->rindex++;
        if (this->rindex > this->size)
            this->rindex = 0;
        return out;
    }
    void push(int v)
    {
        this->buffer[this->windex] = v;
        this->windex++;
        if (this->windex > this->size)
            this->windex = 0;
    }

} Stream;

Stream *CreateStream(uint32_t len);
void DestoryStream(Stream *stream);