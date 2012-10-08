/**
 a queue implementation in C using a dynamic array
    author: Zhiliang Su (zsu2[at]buffalo[dot]edu)
    revision: 07 Oct 2012
*/
#ifndef _HEADER_QUEUE_
#define _HEADER_QUEUE_

#include <stdio.h>
#include <stdlib.h>

struct queue_entity{
    void ** _queue_array_;
    int _queue_idx_head_;
    int _queue_idx_last_;
    int _queue_num_element_;
    int _queue_size_;
};

/**
 function: queue_init
  method to initialize a queue by passing a desired size for the queue
  input:
    struct queue_entity * : a pointer to the queue entity
    int size: size
  output:
    int: 0 for normal otherwise abnormal
*/
int queue_init(struct queue_entity * ptr_queue_entity, int size);

/**
 function: queue_enqueue
  method to add an element into the queue
  input:
    struct queue_entity * : a pointer to the target queue entity
    void *: a void pointer pointing to the to-be-added object
  output:
    int : 1 indicates success, 0 for unsuccess
*/
int queue_enqueue(struct queue_entity * ptr_queue_entity, void * new_data);

/**
 function: queue_dequeue
  method to pop the head data off the queue
  input:
    struct queue_entity * : a pointer to the target queue entity
  output:
    void * : pointer to that specific head data. NULL if unsuccess
*/
void * queue_dequeue(struct queue_entity * ptr_queue_entity);

/**
 function: queue_free
  to free the memories used by the queue
  input:
    struct queue_entity * : a pointer to the target queue entity
*/
void queue_free(struct queue_entity * ptr_queue_entity);

/**
 function: queue_is_full 
  to test if the queue is full
  input:
    struct queue_entity * : a pointer to the target queue entity
  output:
    1 for full, 0 no
*/
int queue_is_full(struct queue_entity * ptr_queue_entity);

/**
 function: queue_is_empty
  to test if the queue is empty
  input:
    struct queue_entity * : a pointer to the target queue entity
  output:
    1 for empty, 0 no
*/
int queue_is_empty(struct queue_entity * ptr_queue_entity);
#endif
