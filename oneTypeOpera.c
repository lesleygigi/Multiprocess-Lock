#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "include/hash.h"

#define NUM_KEYS 10000
#define MAX_NUM_THREADS 20

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

void* lookup(void* arg) {
    int thread_id=*(int *)arg;
    for (int i = 0; i < NUM_KEYS; i++) {
        hash_lookup(my_hash,thread_id*NUM_KEYS+i);
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
int main() {
    int bucket_size = 200; // 哈希表桶的大小
    my_hash = hash_new(bucket_size);
    pthread_t threads[MAX_NUM_THREADS];
    int thread_ids[MAX_NUM_THREADS];
    struct timeval insert_start,delete_start,lookup_start;
    struct timeval insert_end,delete_end, lookup_end;

    for(int i=4;i<=MAX_NUM_THREADS;i+=2){
        gettimeofday(&insert_start,NULL);
        for(int j=0;j<i;j++){
            pthread_create(&threads[j],NULL, insert, &thread_ids[j]);
        }
        for(int j=0;j<i;j++){
            pthread_join(threads[j], NULL);
        }
        gettimeofday(&insert_end,NULL);

        gettimeofday(&lookup_start,NULL);
        for(int j=0;j<i;j++){
            pthread_create(&threads[j], NULL, lookup, &thread_ids[j]);
        }
        for(int j=0;j<i;j++){
            pthread_join(threads[j], NULL);
        }
        gettimeofday(&lookup_end,NULL);

        gettimeofday(&delete_start,NULL);
        for(int j=0;j<i;j++){
            pthread_create(&threads[j], NULL, delete, &thread_ids[j]);
        }
        for(int j=0;j<i;j++){
            pthread_join(threads[j], NULL);
        }
        gettimeofday(&delete_end,NULL);
        double insert_elapsedTime=getElapsedTime(insert_start,insert_end);
        double delete_elapsedTime=getElapsedTime(delete_start,delete_end);
        double lookup_elapsedTime=getElapsedTime(lookup_start,lookup_end);
        printf("%d threads insert_elapsedTime:%.6lf,\n",i,insert_elapsedTime);
        printf("%d threads delete_elapsedTime:%.6lf,\n",i,delete_elapsedTime);
        printf("%d threads lookup_elapsedTime:%.6lf,\n",i,lookup_elapsedTime);
    }
    
    hash_destroy(my_hash);

    return 0;
}
