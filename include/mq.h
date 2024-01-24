#ifndef __MQ_H__
#define __MQ_H__
#include <pthread.h>
struct mq_t{
    int *buf;
    int buf_size;
    int fill_ptr;
    int use_ptr;
    int count;
    
    pthread_cond_t empty;
    pthread_cond_t fill;
    pthread_mutex_t mutex;
};

struct mq_t *mq_new(int buf_size);
void mq_destroy(struct mq_t *mq);

void mq_produce(struct mq_t *mq, int item);
int mq_consume(struct mq_t *mq);

#endif