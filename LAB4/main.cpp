#include <iostream>
#include "buffer.h"


Buffer dough_buffer, meat_buffer, cottage_buffer, lettuce_buffer;


void* buffer_states(void* arguments)
{
    while(1)
    {
        sleep(3);
        printf("Stany buforow:\nciasto: %d \nmieso: %d \nser: %d \nkapusta: %d\n\n", dough_buffer.get_product_count(), meat_buffer.get_product_count(), 
                cottage_buffer.get_product_count(), lettuce_buffer.get_product_count());
    }
}

void* producer_dough(void* arguments)
{
    while(1)
    {
        sleep(3);
        dough_buffer.produce();
    }
}

void* producer_meat(void* arguments)
{
    while(1)
    {
        sleep(3);
        meat_buffer.produce();
    }
}

void* producer_cottage(void* arguments)
{
    while(1)
    {
        sleep(3);
        cottage_buffer.produce();
    }
}

void* producer_lettuce(void* arguments)
{
    while(1)
    {
        sleep(3);
        lettuce_buffer.produce();
    }
}

void* consumer_meat(void* arguments)
{
    while(1)
    {
        sleep(3);
        meat_buffer.consume();
        dough_buffer.consume();
        printf("Wyprodukowano 1 pierogi z miesem\n");
    }
}

void* consumer_cottage(void* arguments)
{
    while(1)
    {
        sleep(3);
        cottage_buffer.consume();
        dough_buffer.consume();
        printf("Wyprodukowano 1 pierogi z serem\n");
    }
}

void* consumer_lettuce(void* arguments)
{
    while(1)
    {
        sleep(3);
        lettuce_buffer.consume();
        dough_buffer.consume();
        printf("Wyprodukowano 1 pierogi z kapusta\n");
    }
}

int check_arguments(int argc, char *argv[])
{
    int i = argc-1;
    if (argc != 8)
    {
        printf("Invalid amount of arguments\n");
        return -1;
    }
    while (i > 0)
    {
        if (atoi(argv[i]) < 0 || strcmp(argv[i], "0") == 0)
        {
            printf("Thread amount cannot be zero or negative\n");
            return -1;
        }
        else if (atoi(argv[i]) == 0)
        {
            printf("Incorrect thread number\n");
            return -1;
        }
        i--;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (check_arguments(argc, argv) == -1)
    {
        return -1;
    }
    int producer_dough_amount = atoi(argv[1]);
    int producer_meat_amount = atoi(argv[2]);
    int producer_cottage_amount = atoi(argv[3]);
    int producer_lettuce_amount = atoi(argv[4]);
    int consumer_meat_amount = atoi(argv[5]);
    int consumer_cottage_amount = atoi(argv[6]);
    int consumer_lettuce_amount = atoi(argv[7]);

    pthread_t threads_producers_dough[producer_dough_amount];
    pthread_t threads_producers_meat[producer_meat_amount];
    pthread_t threads_producers_cottage[producer_cottage_amount];
    pthread_t threads_producers_lettuce[producer_lettuce_amount];
    pthread_t threads_consumers_meat[consumer_meat_amount];
    pthread_t threads_consumers_cottage[consumer_cottage_amount];
    pthread_t threads_consumers_lettuce[consumer_lettuce_amount];
    pthread_t threads_buffer_states[1];

    for (int i = 0; i < producer_dough_amount; ++i)
    {
        pthread_create(&threads_producers_dough[i], NULL, &producer_dough, NULL);
    }
    for (int i = 0; i < producer_meat_amount; ++i)
    {
        pthread_create(&threads_producers_meat[i], NULL, &producer_meat, NULL);
    }
    for (int i = 0; i < producer_cottage_amount; ++i)
    {
        pthread_create(&threads_producers_cottage[i], NULL, &producer_cottage, NULL);
    }
    for (int i = 0; i < producer_lettuce_amount; ++i)
    {
        pthread_create(&threads_producers_lettuce[i], NULL, &producer_lettuce, NULL);
    }

    for (int i = 0; i < consumer_meat_amount; ++i)
    {
        pthread_create(&threads_consumers_meat[i], NULL, &consumer_meat, NULL);
    }
    for (int i = 0; i < consumer_cottage_amount; ++i)
    {
        pthread_create(&threads_consumers_cottage[i], NULL, &consumer_cottage, NULL);
    }
    for (int i = 0; i < consumer_lettuce_amount; ++i)
    {
        pthread_create(&threads_consumers_lettuce[i], NULL, &consumer_lettuce, NULL);
    }

    pthread_create(&threads_buffer_states[0], NULL, &buffer_states, NULL);
    pthread_join(threads_buffer_states[0], NULL);

    for (int i = consumer_lettuce_amount -1; i >= 0; --i) 
    {
        pthread_join(threads_consumers_lettuce[i], NULL);
    }
    for (int i = consumer_cottage_amount -1; i >= 0; --i)
    {
        pthread_join(threads_consumers_cottage[i], NULL);
    }
    for (int i = consumer_meat_amount -1; i >= 0; --i) 
    {
        pthread_join(threads_consumers_meat[i], NULL);
    }
    
    for (int i = producer_lettuce_amount - 1; i >= 0; --i)
    {
        pthread_join(threads_producers_lettuce[i], NULL);
    }
    for (int i = producer_cottage_amount - 1; i >= 0; --i) 
    {
        pthread_join(threads_producers_cottage[i], NULL);
    }
    for (int i = producer_meat_amount - 1; i >= 0; --i) 
    {
        pthread_join(threads_producers_meat[i], NULL);
    }
    for (int i = producer_dough_amount - 1; i >= 0; --i) 
    {
        pthread_join(threads_producers_dough[i], NULL);
    }
}