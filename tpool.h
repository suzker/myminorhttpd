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

// TODO include the networking worker thread, maybe from the network utility, TBA.
#include "queue.h"
#include "utility.h"
#include "scheduler.h"

/**
 function: tpool_init
  to initialize the thread pool, including creating a fixed number of worker threads, a queue for idle threads and etc.
*/
void tpool_init();

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
#endif
