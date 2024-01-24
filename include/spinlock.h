#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_
#include "xchg.c"

typedef struct{
    volatile unsigned int flag;
}spinlock_t;

void spinlock_init(spinlock_t *lock);
void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

#endif
