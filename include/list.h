#ifndef __LIST_H__
#define __LIST_H__
#include "spinlock.h"
#include "mutex.h"
#include "pthread.h"
typedef struct node_t{
    int key;
    struct node_t *next;
}node_t;
struct list_t{
    node_t *head;
    spinlock_t spinlock;
    mutex_t mutex;
    pthread_mutex_t pmutex;
    pthread_spinlock_t pspin;
};

struct list_t *list_new();
void list_destroy(struct list_t *list);

void list_insert(struct list_t *list, int key);
void list_delete(struct list_t *list, int key);
void *list_lookup(struct list_t *list, int key);

#endif