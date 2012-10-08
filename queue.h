/**
 a queue implementation in C using a dynamic array
    author: Zhiliang Su (zsu2[at]buffalo[dot]edu)
    revision: 07 Oct 2012
*/
#ifndef _HEADER_QUEUE_
#define _HEADER_QUEUE_

#include <stdio.h>
#include <stdlib.h>

/**
 function: queue_init
  method to initialize a queue by passing a desired size for the queue
  input:
    int size: size
  output:
    int: 0 for normal otherwise abnormal
*/
int queue_init(int);

/**
 function: queue_enqueue
  method to add an element into the queue
  input:
    void *: a void pointer pointing to the to-be-added object
  output:
    int : 0 indicates success, otherwise unsuccess
*/
int queue_enqueue(void * new_data);

/**
 function: queue_dequeue
  method to pop the head data off the queue
  output:
    void * : pointer to that specific head data. NULL if unsuccess
*/
void * queue_dequeue();

/**
 function: queue_free
  to free the memories used by the queue
*/
void queue_free();

/**
 function: queue_get_num_elements
  to get the number of elements
*/
int queue_get_num_elements();
#endif
