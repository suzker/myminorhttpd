#include "tpool.h"

struct queue_entity __tpool_q_idle_t;
pthread_mutex_t __tpool_mutex_idle_t, __tpool_mutex_job_slot;
sem_t __tpool_sem_q_full, __tpool_sem_q_empty;
pthread_t * __tpool_t_workers;
pthread_cond_t * __tpool_cond_workers;
pthread_t __tpool_t_assigner;
struct scheduler_job ** __tpool_job_slot;
// function pointer to the worker working core methods
void *(*tpool_working_instruction)(struct scheduler_job *);

void tpool_init(void *(*func)(struct scheduler_job *)){
    queue_init(&__tpool_q_idle_t, arg_thread_num);
    pthread_mutex_init(&__tpool_mutex_idle_t, NULL);
    pthread_mutex_init(&__tpool_mutex_job_slot, NULL);
    sem_init(&__tpool_sem_q_full, 0, 0);
    sem_init(&__tpool_sem_q_empty, 0, arg_thread_num);
    __tpool_t_workers = (pthread_t *)malloc(arg_thread_num * sizeof(pthread_t));
    __tpool_cond_workers = (pthread_cond_t *)malloc(arg_thread_num * sizeof(pthread_cond_t));
    __tpool_job_slot = (struct scheduler_job **)malloc(arg_thread_num * sizeof(struct scheduler_job *));
    tpool_working_instruction = func;

    int i;
    // initialize all condvars
    for (i=0; i<arg_thread_num; ++i){
        pthread_cond_init(&(__tpool_cond_workers[i]), NULL);
    }
    // bootstrap all worker threads
    int *self_id;
    for (i=0; i<arg_thread_num; ++i){
        self_id = (int *)malloc(sizeof(int));
        *self_id = i;
        pthread_create(&(__tpool_t_workers[i]), NULL, tpool_thread_worker, self_id);
    }

    // sleep for queueing delay
    sleep(arg_queue_time);
    
    // start the assigner thread
    pthread_create(&__tpool_t_assigner, NULL, tpool_thread_assigner, NULL);
}

void * tpool_thread_worker(void * arg){
    struct scheduler_job * the_job;
    int * my_id = (int *) arg;
    while (1){
        pthread_mutex_lock(&__tpool_mutex_job_slot);
        pthread_cond_wait(&(__tpool_cond_workers[*my_id]), &__tpool_mutex_job_slot);
        the_job = __tpool_job_slot[*my_id];
        pthread_mutex_unlock(&__tpool_mutex_job_slot);
        tpool_working_instruction(the_job);
        // assume that the return of the "working_instruction" means finished
        tpool_recycle_idle_t(my_id);
    }
}

void * tpool_thread_assigner(void * arg){
    struct scheduler_job * __next_job;
    int * __next_worker_id;
    while (1){
        sem_wait(&__tpool_sem_q_full);
        pthread_mutex_lock(&__tpool_mutex_idle_t);
        __next_job = scheduler_get_job();
        __next_worker_id = queue_dequeue(&__tpool_q_idle_t);
        pthread_mutex_unlock(&__tpool_mutex_idle_t);
        sem_post(&__tpool_sem_q_empty);
        if (__next_job && __next_worker_id){
            pthread_mutex_lock(&__tpool_mutex_job_slot);
            __tpool_job_slot[*__next_worker_id] = __next_job;
            pthread_mutex_unlock(&__tpool_mutex_job_slot);
            // wake up the dedicated worker
            pthread_cond_signal(&(__tpool_cond_workers[*__next_worker_id]));
        } else {
            printf("[ERR] Invalid job or worker thread, skipped. \n");
        }
    }
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

void tpool_destroy(){
    queue_free(&__tpool_q_idle_t);
    pthread_mutex_destroy(&__tpool_mutex_idle_t);
    pthread_mutex_destroy(&__tpool_mutex_job_slot);
    sem_destroy(&__tpool_sem_q_full);
    sem_destroy(&__tpool_sem_q_empty);
    free(__tpool_t_workers);
    free(__tpool_cond_workers);
    free(__tpool_job_slot);
}
