#pragma once
#include <stdint.h>
#include <Memory.h>
extern "C" uint64_t div(uint64_t num, uint32_t div);

void *kernel_clone(void *data, size_t size);

typedef struct List
{

    void **elements;
    uint32_t size;
    uint32_t cap;
    List()
    {
        cap = 0;
        size = 0;
        incCap();
    }
    void *incCap()
    {
        this->elements = (void **)realloc((char *)elements, sizeof(uint32_t) * (size + 40));
        cap += 40;
    }
    void push(void *element)
    {
        if (cap >= size)
            incCap();
        elements[size] = element;
        size++;
    }
    void *pop()
    {
        if(size<=0)
            return nullptr;
        size--;
        return elements[size];
    }
    inline void *at(uint32_t index)
    {
        return elements[index];
    }
    void set(void *element, uint32_t index)
    {
        if (index >= size)
            return;
        elements[index] = element;
    }
    void remove(void *element)
    {
        int index = indexOf(element);
        if (index == -1)
            return;
        for (uint32_t i = index; i < size; i++)
        {
            elements[i] = elements[i + 1];
        }
        size--;
    }
    void removeAt(uint32_t index)
    {
        if (index >= size)
            return;
        for (uint32_t i = index; i < size; i++)
        {
            elements[i] = elements[i + 1];
        }
        size--;
    }
    int indexOf(void *element)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            if (elements[i] == element)
            {
                return i;
            }
        }
        return -1;
    }
    void *end()
    {
        if (size == 0)
            return nullptr;
        return elements[size - 1];
    }
};
