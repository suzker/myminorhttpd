/**
 a heap structure implementation in C using a dynamic array
     author: Zhiliang Su (zsu2@buffalo.edu)
     revision: 06 Oct 2012
*/
#include <stdio.h>
#include <stdlib.h>

struct heap_data {
    void * data;
    long cmp;
};

struct heap_data ** _heap_array_;
int _heap_num_element_ = 0;
int _heap_num_allocated_ = 0;
const int _heap_init_allocate_num_ = 4;

/**
 function: add_to_heap
  add to heap method for the heap structure, will add a heap to the end of the tree and auto reheap
  input:
    heap_data * new_data: pointer to the new heap_node
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
  output:
    int: 0 indicates unsucess otherwise sucess added a new node
*/
int add_to_heap(struct heap_data * new_data, int (*ptCmpFunc)(long *, long *));

/**
 function: pop
  pop the root node out of the heap, will auto reheap after popped
  output:
   heap_node * : a pointer to the root node
*/
struct heap_data * heap_pop(int (*ptCmpFunc)(long *, long *));

/**
 function: _heapify_top_down_ (private)
  a private method to reheap the whole heap after poping from the heap
  input:
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
*/
void _heapify_top_down_(int (*ptCmpFunc)(long *, long *));

/**
 function: _heapify_bottom_up_ (private)
  a private method to reheap from bottom to top may be after insertion
  input:
    ptrCmpFunc *: a function pointer to the comparison function taking 2 long inputs
*/
void _heapify_bottom_up_(int (*ptCmpFunc)(long *, long *));

/**
 function heap_free
  call to free the memory allocated for the heap
*/
void heap_free();
