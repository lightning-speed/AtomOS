#pragma once
#include <stdint.h>
#include <Memory.h>
extern "C" uint64_t div(uint64_t num, uint32_t div);
void *kernel_clone(void *data, size_t size);

class List
{
public:
    void **elements;
    uint32_t size;
    uint32_t cap;
    List();
    void incCap();
    void push(void *element);
    void *pop();
    void *at(uint32_t index);
    uint32_t length();
    void flush();
    void set(void *element, uint32_t index);
    void remove(void *element);
    void removeAt(int index);
    int indexOf(void *element);
    void *end();
};

class SpinLock{
private:
public:
    bool locked;

    SpinLock();
    void acquire();
    void release();
};