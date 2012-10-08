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
    // TODO: start the job emitting thread
}

struct scheduler_job scheduler_create_job(void * job_data, long len){
    struct scheduler_job _new_job;
    _new_job.job_data = job_data;
    _new_job.len = len;
    return _new_job;
}

int scheduler_handle_new_job(struct scheduler_job * new_job){
    int _status;
    int _isempty; // if it's empty before adding the new job, will signal the emit thread
    pthread_mutex_lock(&scheduler_joblist_mutex);
     switch (arg_schedule_mode){
        case MODE_FIFO:
            _isempty = queue_is_empty();
            _status =  queue_enqueue(&scheduler_queue , new_job);
            break;
        case MODE_SJF:
            _isempty = heap_is_empty();
            struct heap_data new_heap_data;
            new_heap_data.data = new_job;
            new_heap_data.cmp = new_job->len;
            _status = heap_push(&scheduler_heap, &new_heap_data, cmp_func);
            break;
        default:
            _isempty = queue_is_empty();
            _status = queue_enqueue(&scheduler_queue , new_job);
            break;
    }

    if (_status && _isempty){
        pthread_cond_signal(&scheduler_nonempty_cond);
    }

    pthread_mutex_unlock(&scheduler_joblist_mutex);
    return _status;
}

void* scheduler_emit_job_thread_routin(void* arg){
    struct scheduler_job * _next_job_ptr;
    while (1){
        pthread_mutex_lock(&scheduler_joblist_mutex);
        if (!scheduler_is_joblist_empty){
            _next_job_ptr = scheduler_pop();
            if (_next_job_ptr){
                // TODO: time to call the "tpool_assign_job"
            }
        } else { // if the joblist is empty, block the scheduler
            pthread_cond_wait(&scheduler_nonempty_cond, &scheduler_joblist_mutex);
        }
        pthread_mutex_unlock(&scheduler_joblist_mutex);
    }
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
    switch (arg_schedule_mode){
        case MODE_FIFO:
            _popped_out = (struct scheduler_job *)(queue_dequeue(&scheduler_queue));
            break;
        case MODE_SJF:
            struct heap_data * = _tmp_heap_data;
            _tmp_heap_data = heap_pop(&scheduler_heap);
            _popped_out = (struct scheduler_job *)(_tmp_heap_data->data);
            break;
        default:
            _popped_out = (struct scheduler_job *)(queue_dequeue(&scheduler_queue));
            break;
    }
    return _popped_out;
}
