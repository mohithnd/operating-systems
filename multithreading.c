#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 20
#define ARRAY_SIZE 100000

typedef struct
{
    int *arr;
    int start;
    int end;
    int result;
} ThreadData;

void *sum_array(void *args)
{
    ThreadData *data = (ThreadData *)args;
    int sum = 0;

    for (int i = data->start; i < data->end; i++)
    {
        sum += data->arr[i];
    }

    data->result = sum;

    return NULL;
}

int main()
{
    int arr[ARRAY_SIZE];
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int total_sum = 0;

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = i + 1;
    }

    int segment_size = ARRAY_SIZE / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].arr = arr;
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, &sum_array, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    printf("Total Sum: %d\n", total_sum);

    return 0;
}