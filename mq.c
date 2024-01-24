#include "include/mq.h"
#include<stdlib.h>

struct mq_t *mq_new(int buf_size){
    struct mq_t *mq=(struct mq_t *)malloc(sizeof(struct mq_t));
    mq->buf=(int *)malloc(buf_size*sizeof(int));
    mq->buf_size=buf_size;
    mq->fill_ptr=0;
    mq->use_ptr=0;
    mq->count=0;

    pthread_cond_init(&mq->empty,NULL);
    pthread_cond_init(&mq->fill,NULL);
    pthread_mutex_init(&mq->mutex,NULL);

    return mq;
}
void mq_destroy(struct mq_t *mq){
    free(mq->buf);
    pthread_cond_destroy(&mq->empty);
    pthread_cond_destroy(&mq->fill);
    pthread_mutex_destroy(&mq->mutex);
    free(mq);
}

void mq_produce(struct mq_t *mq, int item){
    pthread_mutex_lock(&mq->mutex);
    
    while(mq->count==mq->buf_size){
        pthread_cond_wait(&mq->empty,&mq->mutex);
    }
    mq->buf[mq->fill_ptr]=item;
    mq->fill_ptr=(mq->fill_ptr+1)%mq->buf_size;
    mq->count++;
    pthread_cond_signal(&mq->fill);

    pthread_mutex_unlock(&mq->mutex);
}
int mq_consume(struct mq_t *mq){
    int item;
    pthread_mutex_lock(&mq->mutex);

    while(mq->count==0){
        pthread_cond_wait(&mq->fill,&mq->mutex);
    }
    item=mq->buf[mq->use_ptr];
    mq->use_ptr=(mq->use_ptr+1)%mq->buf_size;
    mq->count--;
    pthread_cond_signal(&mq->empty);
    
    pthread_mutex_unlock(&mq->mutex);
    return item;
}