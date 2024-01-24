//Fairness of My Locks
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <pthread.h>

#include "include/counter.h"

#define MAX_NUM_THREADS 20
#define EXETIME 10000

struct counter_t *counter;
int thread_get_lock[MAX_NUM_THREADS];

void* execute(void* arg) {
    int thread_id=*(int *)arg;

    for (int i = 0; i < EXETIME*10; i++) {
        if(counter_get_value(counter)>0){
            counter_decrement(counter);
            thread_get_lock[thread_id]++;
        }else{
            return NULL;
        }
    }
    return NULL;
}

int main() {
    counter=counter_new(EXETIME);
    pthread_t threads[MAX_NUM_THREADS];
    int thread_ids[MAX_NUM_THREADS];
    for (int i = 0; i < MAX_NUM_THREADS; i++) {
        thread_ids[i]=i;
        pthread_create(&threads[i], NULL, execute, &thread_ids[i]);
    }
    for (int i=0; i<MAX_NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i=0; i<MAX_NUM_THREADS; i++) {
        printf("Thread %d executed %d times\n", i, thread_get_lock[i]);
    }    
    
    counter_destroy(counter);

    return 0;
}