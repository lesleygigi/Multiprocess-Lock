#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/counter.h"

#define NUM_THREADS 5
#define N 100

struct counter_t *my_counter;

void *increment_thread(void *arg) {
    for (int i = 0; i < N; i++) {
        counter_increment(my_counter);
    }
    return NULL;
}

void *decrement_thread(void *arg) {
    for (int i = 0; i < N; i++) {
        counter_decrement(my_counter);
    }
    return NULL;
}

int main() {
    my_counter = counter_new(0);

    pthread_t increment_threads[NUM_THREADS];
    pthread_t decrement_threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&increment_threads[i], NULL, increment_thread, NULL);
        pthread_create(&decrement_threads[i], NULL, decrement_thread, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(increment_threads[i], NULL);
        pthread_join(decrement_threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter_get_value(my_counter));

    counter_destroy(my_counter);

    return 0;
}
