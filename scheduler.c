#include "scheduler.h"

const int scheduler_max_wait = 2000; // maximum number of waiting conns, no new socket conn will be taken if this limit is reached
struct queue_entity scheduler_queue;
struct heap_entity scheduler_heap;

pthread_mutex_t scheduler_joblist_mutex;
pthread_t scheduler_thread;
//def of external variable
sem_t scheduler_sem_full, scheduler_sem_empty;

void scheduler_init(){
    pthread_mutex_init(&scheduler_joblist_mutex, NULL);
    sem_init(&scheduler_sem_full, 0, 0);
    sem_init(&scheduler_sem_empty, 0, scheduler_max_wait);

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
    // TODO start a standalone thread here for scheduler
}

struct scheduler_job scheduler_create_job(void * job_data, long len){
    struct scheduler_job _new_job;
    _new_job.job_data = job_data;
    _new_job.len = len;
    return _new_job;
}

int scheduler_add_job(struct scheduler_job * new_job){
    int _status;
    struct heap_data * new_heap_data;
    sem_wait(&scheduler_sem_empty);
    pthread_mutex_lock(&scheduler_joblist_mutex);
     switch (arg_schedule_mode){
        case MODE_FIFO:
            _status =  queue_enqueue(&scheduler_queue , new_job);
            break;
        case MODE_SJF:
            new_heap_data = (struct heap_data *)malloc(sizeof(struct heap_data));
            new_heap_data->data = new_job;
            new_heap_data->cmp = new_job->len;
            _status = heap_push(&scheduler_heap, new_heap_data, cmp_func);
            break;
        default:
            _status = queue_enqueue(&scheduler_queue , new_job);
            break;
    }
    pthread_mutex_unlock(&scheduler_joblist_mutex);
    sem_post(&scheduler_sem_full);
    return _status;
}

int cmp_func(long *a, long *b){
    int rtr = *b - *a;
    return rtr;
}

void scheduler_destroy(){
    switch (arg_schedule_mode){
        case MODE_FIFO:
            queue_free(&scheduler_queue);
            break;
        case MODE_SJF:
            heap_free(&scheduler_heap);
            break;
        default:
            queue_free(&scheduler_queue);
            break;
    }
    // pthread realted objects
    pthread_mutex_destroy(&scheduler_joblist_mutex);
    sem_destroy(&scheduler_sem_full);
    sem_destroy(&scheduler_sem_empty);
}

int scheduler_is_joblist_empty(){
    switch (arg_schedule_mode){
        case MODE_FIFO:
            return queue_is_empty(&scheduler_queue);
            break;
        case MODE_SJF:
            return heap_is_empty(&scheduler_heap);
            break;
        default:
            return queue_is_empty(&scheduler_queue);
            break;
    }
}


struct scheduler_job * scheduler_pop(){
    struct scheduler_job * _popped_out;
    struct heap_data * _tmp_heap_data;
    switch (arg_schedule_mode){
        case MODE_FIFO:
            _popped_out = (struct scheduler_job *)(queue_dequeue(&scheduler_queue));
            break;
        case MODE_SJF:
            _tmp_heap_data = heap_pop(&scheduler_heap, cmp_func);
            _popped_out = (struct scheduler_job *)(_tmp_heap_data->data);
            break;
        default:
            _popped_out = (struct scheduler_job *)(queue_dequeue(&scheduler_queue));
            break;
    }
    return _popped_out;
}
