// producer-consumer problem, 1 producer and n consumer
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "mq.h"

int max;
int loops;
int consumers = 1;
struct mq_t *mq;

void *producer(void *arg) {
    for (int i = 1; i <= loops; i++)
        mq_produce(mq, i);

    // end case: put an end-of-production marker (-1)
    // into shared buffer, one per consumer
    for (int i = 0; i < consumers; i++)
        mq_produce(mq, -1);

    return NULL;
}

void *consumer(void *arg) {
    int data = 0;
    // consumer: keep pulling data out of shared buffer
    // until you receive a -1 (end-of-production marker)
    while ((data = mq_consume(mq)) != -1)
        printf("%d\n", data);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
        exit(1);
    }
    max = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);

    mq = mq_new(max);

    pthread_t pid, cid[consumers];
    pthread_create(&pid, NULL, producer, NULL);
    for (int i = 0; i < consumers; i++) {
        pthread_create(&cid[i], NULL, consumer, (void *)(long long int)i);
    }

    pthread_join(pid, NULL);
    for (int i = 0; i < consumers; i++) {
        pthread_join(cid[i], NULL);
    }

    mq_destroy(mq);

    return 0;
}
