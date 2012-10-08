/**
 a heap structure implementation in C using a dynamic array
     author: Zhiliang Su (zsu2[at]buffalo[dot]edu)
     revision: 06 Oct 2012
*/
#ifndef _HEADER_HEAP_
#define _HEADER_HEAP_
#include <stdio.h>
#include <stdlib.h>

struct heap_data {
    void * data;
    long cmp;
};

struct heap_entity {
    struct heap_data ** _heap_array_;
    int _heap_num_element_;
    int _heap_num_allocated_;
};

/**
 function: heap_init
  to initialize a heap
  input:
    struct heap_entity * : a pointer to the target heap
*/
void heap_init(struct heap_entity * heap_ptr_entity);

/**
 function: heap_push
  add to heap method for the heap structure, will add a heap to the end of the tree and auto reheap
  input:
    struct heap_entity * : a pointer to the target heap
    heap_data * new_data: pointer to the new heap_data
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
  output:
    int: 0 indicates unsucess otherwise sucess added a new node
*/
int heap_push(struct heap_entity * heap_ptr_entity, struct heap_data * new_data, int (*ptCmpFunc)(long *, long *));

/**
 function: pop
  pop the root node out of the heap, will auto reheap after popped
  input:
    struct heap_entity * : a pointer to the target heap
    ptCmpFunc *: a function pointer to the comparison function taking 2 long inputs
  output:
    heap_data * : a pointer to the root node
*/
struct heap_data * heap_pop(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *, long *));

/**
 function: _heapify_top_down_ (private)
  a private method to reheap the whole heap after poping from the heap
  input:
    struct heap_entity * : a pointer to the target heap
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
*/
void _heapify_top_down_(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *, long *));

/**
 function: _heapify_bottom_up_ (private)
  a private method to reheap from bottom to top may be after insertion
  input:
    struct heap_entity * : a pointer to the target heap
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
*/
void _heapify_bottom_up_(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *, long *));

/**
 function heap_free
  call to free the memory allocated for the heap
  input:
    struct heap_entity * : a pointer to the target heap
*/
void heap_free(struct heap_entity * heap_ptr_entity);

/**
 function heap_is_empty
  method to check if the heap is empty
  output:
    0 for not empty while 1 for is empty
*/
int heap_is_empty();
#endif
