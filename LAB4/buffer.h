#include <semaphore.h>
#include "monitor.h"

#define N 10

class Buffer: public Monitor
{
    public:
    Buffer();
    void produce();
    void consume();
    int get_product_count() {return product_count;}

    private:
    int size;
    int product_count;
    int product_buffer[N];
    Condition full, empty;
};