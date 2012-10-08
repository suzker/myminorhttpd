#include "scheduler.h"

const int scheduler_max_wait = 2000; // maximum number of waiting conns, no new socket conn will be taken if this limit is reached
struct queue_entity scheduler_queue;
struct heap_entity scheduler_heap;

pthread_mutex_t scheduler_joblist_mutex;
pthread_cond_t scheduler_nonempty_cond;
pthread_t scheduler_thread;

void scheduler_init(){
    pthread_mutex_init(&scheduler_joblist_mutex, NULL);
    pthread_cond_init(&scheduler_nonempty_cond, NULL);

    switch (arg_schedule_mode){
        case MODE_FIFO:
            queue_init(&scheduler_queue, scheduler_max_wait);
            break;
        case MODE_SJF:
            heap_init(&scheduler_heap);
            break;
        default:
            queue_init(&scheduler_queue, scheduler_max_wait);
            break;
    }
}

int scheduler_handle_new_job(struct scheduler_job * new_job){
    int _status;
    pthread_mutex_lock(&scheduler_joblist_mutex);
     switch (arg_schedule_mode){
        case MODE_FIFO:
            _status =  queue_enqueue(&scheduler_queue , new_job);
            break;
        case MODE_SJF:
            struct heap_data new_heap_data;
            new_heap_data.data = new_job;
            new_heap_data.cmp = new_job->len;
            _status = heap_push(&scheduler_heap, &new_heap_data, cmp_func);
            break;
        default:            
            _status = queue_enqueue(&scheduler_queue , new_job);
            break;
    }
    pthread_mutex_unlock(&scheduler_joblist_mutex);
    if (_status){
        pthread_cond_signal(&scheduler_nonempty_cond);
    }
    return _status;
}

void* scheduler_emit_job_thread_routin(void* arg){

}

int cmp_func(long *a, long *b){
    int rtr = *b - *a;
    return rtr;
}


