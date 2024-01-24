#include "include/counter.h"

struct counter_t *counter_new(int value){
    struct counter_t *c=(struct counter_t *)malloc(sizeof(struct counter_t));
    c->value=value;
    spinlock_init(&(c->spinlock));
    return c;
}
void counter_destroy(struct counter_t *c){
    if(c!=NULL){
        free(c);
    }
}

int counter_get_value(struct counter_t *c){
    int ret;
    do{
        spinlock_acquire(&c->spinlock);
        ret=c->value;
        spinlock_release(&c->spinlock);
    }while(ret!=c->value);

    return ret;
}
void counter_increment(struct counter_t *c){
    spinlock_acquire(&c->spinlock);
    c->value++;
    spinlock_release(&c->spinlock);
}
void counter_decrement(struct counter_t *c){
    spinlock_acquire(&c->spinlock);
    c->value--;
    spinlock_release(&c->spinlock);
}








// struct counter_t *counter_new(int value){
//     struct counter_t *c=(struct counter_t *)malloc(sizeof(struct counter_t));
//     c->value=value;
//     mutex_init(&(c->mutex));
//     return c;
// }
// void counter_destroy(struct counter_t *c){
//     if(c!=NULL){
//         free(c);
//     }
// }

// int counter_get_value(struct counter_t *c){
//     int ret;
//     do{
//         mutex_acquire(&c->mutex);
//         ret=c->value;
//         mutex_release(&c->mutex);
//     }while(ret!=c->value);

//     return ret;
// }
// void counter_increment(struct counter_t *c){
//     mutex_acquire(&c->mutex);
//     c->value++;
//     mutex_release(&c->mutex);
// }
// void counter_decrement(struct counter_t *c){
//     mutex_acquire(&c->mutex);
//     c->value--;
//     mutex_release(&c->mutex);
// }








// struct counter_t *counter_new(int value){
//     struct counter_t *c=(struct counter_t *)malloc(sizeof(struct counter_t));
//     c->value=value;
//     pthread_spin_init(&(c->pspin));
//     return c;
// }
// void counter_destroy(struct counter_t *c){
//     if(c!=NULL){
//         free(c);
//     }
// }

// int counter_get_value(struct counter_t *c){
//     int ret;
//     do{
//         pthread_spin_lock(&c->pspin);
//         ret=c->value;
//         pthread_spin_unlock(&c->pspin);
//     }while(ret!=c->value);

//     return ret;
// }
// void counter_increment(struct counter_t *c){
//     pthread_spin_lock(&c->pspin);
//     c->value++;
//     pthread_spin_unlock(&c->pspin);
// }
// void counter_decrement(struct counter_t *c){
//     pthread_spin_lock(&c->pspin);
//     c->value--;
//     pthread_spin_unlock(&c->pspin);
// }









// struct counter_t *counter_new(int value){
//     struct counter_t *c=(struct counter_t *)malloc(sizeof(struct counter_t));
//     c->value=value;
//     pthread_mutex_init(&(c->pmutex));
//     return c;
// }
// void counter_destroy(struct counter_t *c){
//     if(c!=NULL){
//         free(c);
//     }
// }

// int counter_get_value(struct counter_t *c){
//     int ret;
//     do{
//         pthread_mutex_lock(&c->pmutex);
//         ret=c->value;
//         pthread_mutex_unlock(&c->pmutex);
//     }while(ret!=c->value);

//     return ret;
// }
// void counter_increment(struct counter_t *c){
//     pthread_mutex_lock(&c->pmutex);
//     c->value++;
//     pthread_mutex_unlock(&c->pmutex);
// }
// void counter_decrement(struct counter_t *c){
//     pthread_mutex_lock(&c->pmutex);
//     c->value--;
//     pthread_mutex_unlock(&c->pmutex);
// }