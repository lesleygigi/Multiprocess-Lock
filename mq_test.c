#include "include/mq.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 10

struct test_data {
    struct mq_t *mq;
    int id;
};

void *producer(void *arg) {
    struct test_data *data = (struct test_data *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        mq_produce(data->mq, data->id * NUM_ITEMS + i);
        printf("Producer %d produced: %d\n", data->id, data->id * NUM_ITEMS + i);
        usleep(rand() % 100000);  // Simulate some work
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    struct test_data *data = (struct test_data *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = mq_consume(data->mq);
        printf("Consumer %d consumed: %d\n", data->id, item);
        usleep(rand() % 100000);  // Simulate some work
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    struct mq_t *mq = mq_new(BUF_SIZE);

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    struct test_data producer_data[NUM_PRODUCERS];
    struct test_data consumer_data[NUM_CONSUMERS];

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_data[i].mq = mq;
        producer_data[i].id = i;
        pthread_create(&producers[i], NULL, producer, (void *)&producer_data[i]);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_data[i].mq = mq;
        consumer_data[i].id = i;
        pthread_create(&consumers[i], NULL, consumer, (void *)&consumer_data[i]);
    }

    // Join producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    mq_destroy(mq);

    return 0;
}
