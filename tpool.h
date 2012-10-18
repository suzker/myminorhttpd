/**
 A thread pool implementation
 Author: Zhiliang SU
 Revised: 18 Oct, 2012
*/
#ifndef _HEADER_TPOOL_
#define _HEADER_TPOOL_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "utility.h"
#include "scheduler.h"

/**
 function: tpool_init
  to initialize the thread pool, including creating a fixed number of worker threads, a queue for idle threads and etc.
  input:
    void *(*func)(void * arg) : the working instruction method pointer
*/
void tpool_init(void * (*func)(struct scheduler_job *));

/**
 function: tpool_thread_worker
  a worker thread... will call the "working_instruction" method. An arg will be passed in as the worker id which is an int*.
  each worker should call the recycle method "tpool_recycle_idle_t" before putting itself to sleep by waititng on it's own condition.
  after the worker is invoked, it should check the job slot to see if the job there is NULL and if not will pass the job on to the working_instruction function
*/
void * tpool_thread_worker(void *);

/**
 function: tpool_thread_assigner
  a job assigner to keep getting the idle thread and try to consume from the scheduler's job list by calling the producer method from scheduler
  will implemented as a consumer inside a consumer
*/
void * tpool_thread_assigner(void *);

/**
 function: tpool_recycle_idle_t
  to enqueue an index of the idle worker thread
  input:
    int idx_idle_t; the index of the calling worker thread
  output:
    int: 0 indicates unsuccessful otherwise success
*/
int tpool_recycle_idle_t(int * idx_idle_t);

/**
 function: tpool_destroy
  to release memories used by the tpool
*/
void tpool_destroy();

#endif
