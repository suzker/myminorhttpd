/**
 a queue implementation in C using a dynamic array
    author: Zhiliang Su (zsu2[at]buffalo[dot]edu)
    revision: 07 Oct 2012
*/
#include <stdio.h>
#include <stdlib.h>

void ** _queue_array_;
int _queue_num_element_ = 0;
int _queue_num_allocated_ = 0;
const int _queue_init_allocate_num_ = 4;

/**
 function: enqueue
  method to add an element into the queue
  input:
    void *: a void pointer pointing to the to-be-added object
  output:
    int : 0 indicates success, otherwise unsuccess
*/
int enqueue(void * new_data);

/**
 function: dequeue
  method to pop the head data off the queue
  output:
    void * : pointer to that specific head data. NULL if unsuccess
*/
void * dequeue();

/**
 function: queue_free
  to free the memories used by the queue
*/
void queue_free();
