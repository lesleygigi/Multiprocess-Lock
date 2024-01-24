#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "include/hash.h"

#define MAX_NUM_THREADS 20
#define N 5000
#define HASH_SIZE 10

struct hash_t *my_hash;

double getElapsedTime(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void* insert(void* arg) {
    int thread_id=*(int *)arg;

    for (int i = 0; i < N; i++) {
        hash_insert(my_hash,thread_id*N+i);
    }

    return NULL;
}
void* delete(void* arg) {
    int thread_id=*(int *)arg;

    for (int i = 0; i < N; i++) {
        hash_delete(my_hash,thread_id*N+i);
    }

    return NULL;
}

int main() {
    my_hash=hash_new(HASH_SIZE);

    pthread_t threads[MAX_NUM_THREADS];
    int thread_ids[MAX_NUM_THREADS];

    for(int i=4;i<=MAX_NUM_THREADS;i+=2){
        struct timeval start,end;
        
        gettimeofday(&start,NULL);

        for (int j=0; j<i; j++) {
            thread_ids[j]=j;
            pthread_create(&threads[j], NULL, insert, &thread_ids[j]);
        }
        for (int j=0; j<i; j++) {
            pthread_join(threads[j], NULL);
        }
        for (int j=0; j<i; j++) {
            thread_ids[j]=j;
            pthread_create(&threads[j], NULL, delete, &thread_ids[j]);
        }
        for (int j=0; j<i; j++) {
            pthread_join(threads[j], NULL);
        }

        gettimeofday(&end,NULL);

        double elapsedTime=getElapsedTime(start,end);
        
        printf("thread nums: %d, elapsed time: %.6lf\n",i,elapsedTime);
    }

    hash_destroy(my_hash);

    return 0;
}
