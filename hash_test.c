#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/hash.h"

#define NUM_THREADS 5
#define N 10
#define HASH_SIZE 10

struct hash_t *my_hash;

void *insert_thread(void *arg) {
    int thread_id = *(int *)arg;
    for (int i = 0; i < N; i++) {
        hash_insert(my_hash, thread_id * N + i);
    }
    return NULL;
}

void *delete_thread(void *arg) {
    int thread_id = *(int *)arg;
    for (int i = 0; i < N; i++) {
        hash_delete(my_hash, thread_id * N + i);
    }
    return NULL;
}

int main() {
    my_hash = hash_new(HASH_SIZE);

    pthread_t insert_threads[NUM_THREADS];
    pthread_t delete_threads[NUM_THREADS];

    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&insert_threads[i], NULL, insert_thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(insert_threads[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&delete_threads[i], NULL, delete_thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(delete_threads[i], NULL);
    }

    // Output hash table content
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("Bucket %d: ", i);
            node_t *ptr=my_hash->bucket[i]->head;
            while(ptr!=NULL){
                printf("%d ",ptr->key);
                ptr=ptr->next;
            }
            printf("\n");
        printf("\n");
    }

    hash_destroy(my_hash);

    return 0;
}
