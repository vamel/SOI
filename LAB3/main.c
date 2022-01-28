#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define N 10

struct Sem
{
    pthread_mutex_t mutex;
    sem_t semEmpty;
    sem_t semFull;
};

struct Sem dough_sem, meat_sem, cottage_sem, lettuce_sem;

int dough_buffer[N];
int meat_buffer[N];
int cottage_buffer[N];
int lettuce_buffer[N];

int dough_count = 0;
int meat_count = 0;
int cottage_count = 0;
int lettuce_count = 0;

void init_buffers()
{
    for (int i = 0; i < N; ++i)
    {
        dough_buffer[i] = 0;
        meat_buffer[i] = 0;
        cottage_buffer[i] = 0;
        lettuce_buffer[i] = 0;
    }
}

void init_semaphores()
{
    pthread_mutex_init(&dough_sem.mutex, NULL);
    pthread_mutex_init(&meat_sem.mutex, NULL);
    pthread_mutex_init(&cottage_sem.mutex, NULL);
    pthread_mutex_init(&lettuce_sem.mutex, NULL); 
    sem_init(&dough_sem.semEmpty, 0, N);
    sem_init(&dough_sem.semFull, 0, 0);
    sem_init(&meat_sem.semEmpty, 0, N);
    sem_init(&meat_sem.semFull, 0, 0);
    sem_init(&cottage_sem.semEmpty, 0, N);
    sem_init(&cottage_sem.semFull, 0, 0);
    sem_init(&lettuce_sem.semEmpty, 0, N);
    sem_init(&lettuce_sem.semFull, 0, 0);
}

void destroy_semaphores()
{
    sem_destroy(&lettuce_sem.semFull);
    sem_destroy(&lettuce_sem.semEmpty);
    sem_destroy(&cottage_sem.semFull);
    sem_destroy(&cottage_sem.semEmpty);
    sem_destroy(&meat_sem.semFull);
    sem_destroy(&meat_sem.semEmpty);
    sem_destroy(&dough_sem.semFull);
    sem_destroy(&dough_sem.semEmpty);
    pthread_mutex_destroy(&lettuce_sem.mutex); 
    pthread_mutex_destroy(&cottage_sem.mutex);
    pthread_mutex_destroy(&meat_sem.mutex);
    pthread_mutex_destroy(&dough_sem.mutex);
}

void* buffer_states()
{
    printf("ciasto: %d \nmieso: %d \nser: %d \nkapusta: %d\n", dough_count, meat_count, cottage_count, lettuce_count);
}

void* producer_dough()
{
    while(1)
    {
    sleep(3);
    sem_wait(&dough_sem.semEmpty);
    pthread_mutex_lock(&dough_sem.mutex);
    dough_buffer[dough_count] = 1;
    dough_count++;
    buffer_states();
    pthread_mutex_unlock(&dough_sem.mutex);
    sem_post(&dough_sem.semFull);
    }
}

void* producer_meat()
{
    while(1)
    {
    sleep(3);
    sem_wait(&meat_sem.semEmpty);
    pthread_mutex_lock(&meat_sem.mutex);
    meat_buffer[meat_count] = 1;
    meat_count++;
    pthread_mutex_unlock(&meat_sem.mutex);
    sem_post(&meat_sem.semFull);
    }
}

void* producer_cottage()
{
    while(1)
    {
    sleep(3);
    sem_wait(&cottage_sem.semEmpty);
    pthread_mutex_lock(&cottage_sem.mutex);
    cottage_buffer[cottage_count] = 1;
    cottage_count++;
    pthread_mutex_unlock(&cottage_sem.mutex);
    sem_post(&cottage_sem.semFull);
    }
}

void* producer_lettuce()
{
    while(1)
    {
    sleep(3);
    sem_wait(&lettuce_sem.semEmpty);
    pthread_mutex_lock(&lettuce_sem.mutex);
    lettuce_buffer[lettuce_count] = 1;
    lettuce_count++;
    pthread_mutex_unlock(&lettuce_sem.mutex);
    sem_post(&lettuce_sem.semFull);
    }
}

void* consumer_meat()
{
    while(1)
    {
    sleep(3);
    sem_wait(&meat_sem.semFull);
    sem_wait(&dough_sem.semFull);
    pthread_mutex_lock(&dough_sem.mutex);
    dough_buffer[dough_count] = 0;
    dough_count--;
    pthread_mutex_lock(&meat_sem.mutex);
    meat_buffer[meat_count] = 0;
    meat_count--;
    printf("Wyprodukowano 1 pierogi z miesem\n");
    pthread_mutex_unlock(&meat_sem.mutex);
    pthread_mutex_unlock(&dough_sem.mutex);
    sem_post(&dough_sem.semEmpty);
    sem_post(&meat_sem.semEmpty);
    }
}

void* consumer_cottage()
{
    while(1)
    {
    sleep(3);
    sem_wait(&cottage_sem.semFull);
    sem_wait(&dough_sem.semFull);
    pthread_mutex_lock(&dough_sem.mutex);
    dough_buffer[dough_count] = 0;
    dough_count--;
    pthread_mutex_lock(&cottage_sem.mutex);
    cottage_buffer[cottage_count] = 0;
    cottage_count--;
    printf("Wyprodukowano 1 pierogi z serem\n");
    pthread_mutex_unlock(&cottage_sem.mutex);
    pthread_mutex_unlock(&dough_sem.mutex);
    sem_post(&dough_sem.semEmpty);
    sem_post(&cottage_sem.semEmpty);
    }
}

void* consumer_lettuce()
{
    while(1)
    {
    sleep(3);
    sem_wait(&lettuce_sem.semFull);
    sem_wait(&dough_sem.semFull);
    pthread_mutex_lock(&dough_sem.mutex);
    dough_buffer[dough_count] = 0;
    dough_count--;
    pthread_mutex_lock(&lettuce_sem.mutex);
    lettuce_buffer[lettuce_count] = 0;
    lettuce_count--;
    printf("Wyprodukowano 1 pierogi z miesem\n");
    pthread_mutex_unlock(&lettuce_sem.mutex);
    pthread_mutex_unlock(&dough_sem.mutex);
    sem_post(&dough_sem.semEmpty);
    sem_post(&lettuce_sem.semEmpty);
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
    init_buffers();
    init_semaphores();
    pthread_t threads_producers_dough[producer_dough_amount];
    pthread_t threads_producers_meat[producer_meat_amount];
    pthread_t threads_producers_cottage[producer_cottage_amount];
    pthread_t threads_producers_lettuce[producer_lettuce_amount];
    pthread_t threads_consumers_meat[consumer_meat_amount];
    pthread_t threads_consumers_cottage[consumer_cottage_amount];
    pthread_t threads_consumers_lettuce[consumer_lettuce_amount];

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
    destroy_semaphores();
    return 0;
}
