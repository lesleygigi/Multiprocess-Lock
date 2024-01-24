#ifndef _MUTEX_H_
#define _MUTEX_H_
typedef struct{
    volatile unsigned int flag;
}mutex_t;

void mutex_init(mutex_t *lock);
void mutex_acquire(mutex_t *lock);
void mutex_release(mutex_t *lock);
#endif