#ifndef _HEADER_SCHEDULER_
#define _HEADER_SCHEDULER_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"
#include "heap.h"
#include "utility.h"

#define MODE_FIFO 0
#define MODE_SJF 1

struct scheduler_job{
    void* blah;
    long* len;
};

/**
 function: scheduler_init
  to initialize a schedular including creating a queue/heap depending on the scheduler type, defining the maximum connections and etc.
  ,init mutex and cond to ensure thread safety. Also will call to start a new thread to emit new jobs to worker threads from thread pool.
*/
void scheduler_init();

/**
 function: scheduler_handle_new_job
*/
int scheduler_handle_new_job(struct scheduler_job * new_job);

/**
 function: cmp_func
  to customize the comparasion between two inputs
*/
int cmp_func(long*, long*);

/**
 function: scheduler_emit_job_thread_routin
  a thread starting routin to emit jobs in the job list container
  while the container is empty, the thread will be put to halt waiting for the pthread_cond_t signal
  eventually, will then access the thread pool to attain a worker thread
*/
void* scheduler_emit_job_thread_routin(void *);
#endif
