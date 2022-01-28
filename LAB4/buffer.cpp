#include <iostream>
#include "buffer.h"
#include "monitor.h"


Buffer::Buffer(void)
{
    size = N;
    product_count = 0;
    for (int i = 0; i < size; i++)
    {
        product_buffer[i] = 0;
    }
};

void Buffer::produce()
{
    enter();
    if (product_count == size)
    {
        wait(full);
    }
    product_buffer[product_count] = 1;
    product_count++;
    if (product_count == 1)
    {
        signal(empty);
    }
    leave();
}

void Buffer::consume()
{
    enter();
    if (product_count == 0)
    {
        wait(empty);
    }
    product_buffer[product_count] = 0;
    product_count--;
    if (product_count == size - 1)
    {
        signal(full);
    }
    leave();
}