/**
 A Scheduler implementation with SJF/FIFO strategy
 Author: Zhiliang Su
 Revised: 18 Oct, 2012
*/
#ifndef _HEADER_SCHEDULER_
#define _HEADER_SCHEDULER_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "heap.h"
#include "utility.h"

#define MODE_FIFO 0
#define MODE_SJF 1

// the job object
struct scheduler_job{
    void* job_data;
    long len;
};

/**
 function: scheduler_init
  to initialize a schedular including creating a queue/heap depending on the scheduler type, defining the maximum connections and etc.
  init mutex and cond to ensure thread safety. Also will call to start a new thread to emit new jobs to worker threads from thread pool.
*/
void scheduler_init();

/**
 function: scheduler_create_job
  to create a struct scheduler_job object for scheduler
  input:
    void * : pointer to the object related to the job.
    long len: job length measurement
  output:
    struct scheduler_job : an entity of the job
*/
struct scheduler_job scheduler_create_job(void *, long);

/**
 function: scheduler_add_job
  to add a job to the job list
  implemented with the "2 semaphores + 1 mutex" solution for the producer-consumer problem. consider this one as the *ONLY* producer.
*/
int scheduler_add_job(struct scheduler_job * new_job);

/**
 function: scheduler_get_job
  to get a job from the job list
  implemented in the semaphore + mutex way. this is the consumer
  output:
   scheduler_job* : the pointer to the popped out job
*/
struct scheduler_job * scheduler_get_job();

/**
 function: cmp_func
  to customize the comparasion between two inputs
*/
int cmp_func(long*, long*);

/**
 function: scheduler_destroy
  method to destroy a scheduler, mainly to deallocate the joblist container
*/
void scheduler_destroy();

/**
 funciton: is_joblist_empty
  to check if the joblist container is empty 
*/
int scheduler_is_joblist_empty();

/**
 function: scheduler_pop
  to pop out a job from the joblist container
  output: 
    struct scheduler_job *: the pointer to the popped out job
*/
struct scheduler_job * scheduler_pop();
#endif
