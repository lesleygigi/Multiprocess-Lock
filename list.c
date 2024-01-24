#include "include/list.h"
#include <stdio.h>

// when use spinlock
struct list_t *list_new(){
    struct list_t *list=(struct list_t *)malloc(sizeof(struct list_t));
    list->head=NULL;
    spinlock_init(&(list->spinlock));
    return list;
}
void list_destroy(struct list_t *list){
    if(list==NULL){
        return;
    }
    node_t *tmp;
    while(list->head!=NULL){
        tmp=list->head;
        list->head=list->head->next;
        free(tmp);
    }
    free(list);
}
void list_insert(struct list_t *list, int key){
    spinlock_acquire(&(list->spinlock));
    node_t *node=(node_t*)malloc(sizeof(node_t));
    node->key=key;
    node->next=list->head;
    list->head=node;
    spinlock_release(&(list->spinlock));
}
void list_delete(struct list_t *list, int key){
    spinlock_acquire(&(list->spinlock));
    node_t *prev=NULL,*node=NULL;
    node_t *ptr=list->head;
    while(ptr!=NULL){
        prev=node;
        node=ptr;
        ptr=ptr->next;
        if(node->key==key){
            break;
        }
    }
    if(node!=NULL){
        if(prev==NULL){
            list->head=node->next;
        }else{
            prev->next=node->next;
        }
        free(node);
    }
    spinlock_release(&(list->spinlock));
}
void *list_lookup(struct list_t *list, int key){
    spinlock_acquire(&(list->spinlock));
    node_t *ptr=list->head;
    int flag=0;
    while(ptr!=NULL){
        if(ptr->key==key){
            flag=1;
            break;
        }
        ptr=ptr->next;
    }
    if(flag){

    }else{

    }
    spinlock_release(&(list->spinlock));
    return NULL;
}















// // when use mutex
// struct list_t *list_new(){
//     struct list_t *list=(struct list_t *)malloc(sizeof(struct list_t));
//     list->head=NULL;
//     mutex_init(&(list->mutex));
//     return list;
// }
// void list_destroy(struct list_t *list){
//     if(list==NULL){
//         return;
//     }
//     node_t *tmp;
//     while(list->head!=NULL){
//         tmp=list->head;
//         list->head=list->head->next;
//         free(tmp);
//     }
//     free(list);
// }
// void list_insert(struct list_t *list, int key){
//     mutex_acquire(&(list->mutex));
//     node_t *node=(node_t*)malloc(sizeof(node_t));
//     node->key=key;
//     node->next=list->head;
//     list->head=node;
//     mutex_release(&(list->mutex));
// }
// void list_delete(struct list_t *list, int key){
//     mutex_acquire(&(list->mutex));
//     node_t *prev=NULL,*node=NULL;
//     node_t *ptr=list->head;
//     while(ptr!=NULL){
//         prev=node;
//         node=ptr;
//         ptr=ptr->next;
//         if(node->key==key){
//             break;
//         }
//     }
//     if(node!=NULL){
//         if(prev==NULL){
//             list->head=node->next;
//         }else{
//             prev->next=node->next;
//         }
//         free(node);
//     }
//     mutex_release(&(list->mutex));
// }
// void *list_lookup(struct list_t *list, int key){
//     mutex_acquire(&(list->mutex));
//     node_t *ptr=list->head;
//     int flag=0;
//     while(ptr!=NULL){
//         if(ptr->key==key){
//             flag=1;
//             break;
//         }
//         ptr=ptr->next;
//     }
//     if(flag){
        
//     }else{

//     }
//     mutex_release(&(list->mutex));
//     return NULL;
// }















// //when use pspin
// struct list_t *list_new(){
//     struct list_t *list=(struct list_t *)malloc(sizeof(struct list_t));
//     list->head=NULL;
//     pthread_spin_init(&(list->pspin),0);
//     return list;
// }
// void list_destroy(struct list_t *list){
//     if(list==NULL){
//         return;
//     }
//     node_t *tmp;
//     while(list->head!=NULL){
//         tmp=list->head;
//         list->head=list->head->next;
//         free(tmp);
//     }
//     free(list);
// }
// void list_insert(struct list_t *list, int key){
//     pthread_spin_lock(&(list->pspin));
//     node_t *node=(node_t*)malloc(sizeof(node_t));
//     node->key=key;
//     node->next=list->head;
//     list->head=node;
//     pthread_spin_unlock(&(list->pspin));
// }
// void list_delete(struct list_t *list, int key){
//     pthread_spin_lock(&(list->pspin));
//     node_t *prev=NULL,*node=NULL;
//     node_t *ptr=list->head;
//     while(ptr!=NULL){
//         prev=node;
//         node=ptr;
//         ptr=ptr->next;
//         if(node->key==key){
//             break;
//         }
//     }
//     if(node!=NULL){
//         if(prev==NULL){
//             list->head=node->next;
//         }else{
//             prev->next=node->next;
//         }
//         free(node);
//     }
//     pthread_spin_unlock(&(list->pspin));
// }
// void *list_lookup(struct list_t *list, int key){
//     pthread_spin_lock(&(list->pspin));
//     node_t *ptr=list->head;
//     int flag=0;
//     while(ptr!=NULL){
//         if(ptr->key==key){
//             flag=1;
//             break;
//         }
//         ptr=ptr->next;
//     }
//     if(flag){

//     }else{

//     }
//     pthread_spin_unlock(&(list->pspin));
//     return NULL;
// }



























// //when use pmutex
// struct list_t *list_new(){
//     struct list_t *list=(struct list_t *)malloc(sizeof(struct list_t));
//     list->head=NULL;
//     pthread_mutex_init(&(list->pmutex),NULL);
//     return list;
// }
// void list_destroy(struct list_t *list){
//     if(list==NULL){
//         return;
//     }
//     node_t *tmp;
//     while(list->head!=NULL){
//         tmp=list->head;
//         list->head=list->head->next;
//         free(tmp);
//     }
//     free(list);
// }
// void list_insert(struct list_t *list, int key){
//     pthread_mutex_lock(&(list->pmutex));
//     node_t *node=(node_t*)malloc(sizeof(node_t));
//     node->key=key;
//     node->next=list->head;
//     list->head=node;
//     pthread_mutex_unlock(&(list->pmutex));
// }
// void list_delete(struct list_t *list, int key){
//     pthread_mutex_lock(&(list->pmutex));
//     node_t *prev=NULL,*node=NULL;
//     node_t *ptr=list->head;
//     while(ptr!=NULL){
//         prev=node;
//         node=ptr;
//         ptr=ptr->next;
//         if(node->key==key){
//             break;
//         }
//     }
//     if(node!=NULL){
//         if(prev==NULL){
//             list->head=node->next;
//         }else{
//             prev->next=node->next;
//         }
//         free(node);
//     }
//     pthread_mutex_unlock(&(list->pmutex));
// }
// void *list_lookup(struct list_t *list, int key){
//     pthread_mutex_lock(&(list->pmutex));
//     node_t *ptr=list->head;
//     int flag=0;
//     while(ptr!=NULL){
//         if(ptr->key==key){
//             flag=1;
//             break;
//         }
//         ptr=ptr->next;
//     }
//     if(flag){

//     }else{

//     }
//     pthread_mutex_unlock(&(list->pmutex));
//     return NULL;
// }