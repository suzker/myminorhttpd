#include "tpool.h"

struct queue_entity __tpool_q_idle_t;
pthread_mutex_t __tpool_mutex_idle_t;
sem_t __tpool_sem_q_full, __tpool_sem_q_empty;
pthread_t * __tpool_t_workers;
pthread_t __tpool_t_assigner;

void tpool_init(){
    queue_init(&__tpool_q_idle_t, arg_thread_num);
    pthread_mutex_init(&__tpool_mutex_idle_t, NULL);
    sem_init(&__tpool_sem_q_full, 0, 0);
    sem_init(&__tpool_sem_q_empty, 0, arg_thread_num);
    __tpool_t_workers = (pthread_t *)malloc(arg_thread_num * sizeof(pthread_t));
    int i;
    for (i=0; i<arg_thread_num; ++i){
        // TODO bootstrap each worker thread in a for loop
    }
}

void * tpool_thread_assigner(void * arg){
    sem_wait(&__tpool_sem_q_full);
    pthread_mutex_lock(&__tpool_mutex_idle_t);
    // TODO call the producer method from the scheduler
    pthread_mutex_unlock(&__tpool_mutex_idle_t);
    sem_post(&__tpool_sem_q_empty);
}

int tpool_recycle_idle_t(int * idx_idle_t){
    sem_wait(&__tpool_sem_q_empty);
    pthread_mutex_lock(&__tpool_mutex_idle_t);
    //safe critical section
    queue_enqueue(&__tpool_q_idle_t, idx_idle_t);
    //end of safe CS
    pthread_mutex_unlock(&__tpool_mutex_idle_t);
    sem_post(&__tpool_sem_q_full);
}
