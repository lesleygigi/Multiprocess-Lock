#include "include/hash.h"
#include "stdlib.h"

int f(int key){
    int bitsize=sizeof(key)*8;
    int h=0;
    for(int i=0;i<bitsize;i++){
        h+=(key>>i)&1;
    }
    return h;
}

struct hash_t *hash_new(int size){
    struct hash_t *hash=(struct hash_t *)malloc(sizeof(struct hash_t));
    hash->bucket = (struct list_t **)malloc(size * sizeof(struct list_t *));
    for(int i=0;i<size;i++){
        hash->bucket[i]=list_new();
    }
    hash->size=size;
    return hash;
}

void hash_destroy(struct hash_t *hash){
    if(hash==NULL){
        return;
    }
    for(int i=0;i<hash->size;i++){
        list_destroy(hash->bucket[i]);
    }
    free(hash);
}

void hash_insert(struct hash_t *hash, int key){
    int h=f(key)%hash->size;
    list_insert(hash->bucket[h],key);
}

void hash_delete(struct hash_t *hash, int key){
    int h=f(key)%hash->size;
    list_delete(hash->bucket[h],key);
}

void *hash_lookup(struct hash_t *hash, int key){
    int h=f(key)%hash->size;
    list_lookup(hash->bucket[h],key);
    return NULL;
}