#include "include/mutex.h"
#include "include/xchg.c"
#include "include/sys_futex.c"
#include<stdio.h>

void mutex_init(mutex_t *lock){
    lock->flag=0;
}

void mutex_acquire(mutex_t *lock){
    while(1){
        if(!xchg(&lock->flag,1)){
            break;
        }
        sys_futex((void*)&lock->flag,FUTEX_WAIT,1,NULL,NULL,0);
    }
}
void mutex_release(mutex_t *lock){
    lock->flag=0;
    sys_futex((void*)&lock->flag,FUTEX_WAKE,1,NULL,NULL,0);
}