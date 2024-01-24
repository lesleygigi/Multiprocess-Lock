#ifndef __COUNTER_H__
#define __COUNTER_H__
#include "spinlock.h"
#include "mutex.h"
#include "pthread.h"
struct counter_t{
    volatile unsigned int value;
    spinlock_t spinlock;
    mutex_t mutex;
    pthread_spinlock_t pspin;
    pthread_mutex_t pmutex;
};

struct counter_t *counter_new(int value);
void counter_destroy(struct counter_t *c);

int counter_get_value(struct counter_t *c);
void counter_increment(struct counter_t *c);
void counter_decrement(struct counter_t *c);

#endif