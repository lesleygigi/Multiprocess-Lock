#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/list.h"

#define NUM_THREADS 5
#define N 1000

struct list_t *my_list;

void *insert_thread(void *arg) {
    int thread_id = *(int *)arg;
    for (int i = 0; i < N; i++) {
        list_insert(my_list, thread_id * N + i);
    }
    return NULL;
}

void *delete_thread(void *arg) {
    int thread_id = *(int *)arg;
    for (int i = 0; i < N; i++) {
        list_delete(my_list, thread_id * N + i);
    }
    return NULL;
}

int main() {
    my_list = list_new();

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
    node_t *ptr=my_list->head;
    while(ptr!=NULL){
        printf("%d ",ptr->key);
        ptr=ptr->next;
    }
    printf("\n");
    list_destroy(my_list);

    return 0;
}
