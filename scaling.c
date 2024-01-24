#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "include/hash.h"

#define NUM_THREADS 100
#define NUM_KEYS 1000

struct hash_t *my_hash;

double getElapsedTime(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void* insert(void* arg) {
    int thread_id=*(int *)arg;
    for (int i = 0; i < NUM_KEYS; i++) {
        hash_insert(my_hash,thread_id*NUM_KEYS+i);
    }
    return NULL;
}

void* delete(void* arg) {
    int thread_id=*(int *)arg;

    for (int i = 0; i < NUM_KEYS; i++) {
        hash_delete(my_hash,thread_id*NUM_KEYS+i);
    }

    return NULL;
}

void* lookup(void* arg) {
    int thread_id=*(int *)arg;

    for (int i = 0; i < NUM_KEYS; i++) {
        hash_lookup(my_hash,thread_id*NUM_KEYS+i);
    }

    return NULL;
}

int main() {
    int bucket_sizes[] = {10, 20, 50, 100, 200, 500};
    struct timeval start, end;
    double total_time;

    for (int b = 0; b < sizeof(bucket_sizes) / sizeof(bucket_sizes[0]); b++) {
        int bucket_size = bucket_sizes[b];
        printf("Testing with %d buckets\n", bucket_size);

        my_hash = hash_new(bucket_size);
        pthread_t threads[NUM_THREADS];
        int thread_ids[NUM_THREADS];

        gettimeofday(&start, NULL);

        for (int i = 0; i < NUM_THREADS; i++) {
            thread_ids[i] = i;
            pthread_create(&threads[i], NULL, insert, &thread_ids[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

                for (int i = 0; i < NUM_THREADS; i++) {
            thread_ids[i] = i;
            pthread_create(&threads[i], NULL, delete, &thread_ids[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        gettimeofday(&end, NULL);

        total_time = getElapsedTime(start,end);
        printf("Time taken with %d buckets: %f seconds\n", bucket_size, total_time);
        fflush(stdout);
        hash_destroy(my_hash);
        my_hash=NULL;
    }

    return 0;
}
