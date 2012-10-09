#include "heap.h"

const int _heap_init_allocate_num_ = 4;

void heap_init(struct heap_entity * heap_ptr_entity){
    heap_ptr_entity->_heap_num_element_ = 0;
    heap_ptr_entity->_heap_num_allocated_ = 0;
    heap_ptr_entity->_heap_array_ = NULL;
}

int heap_push(struct heap_entity * heap_ptr_entity, struct heap_data * new_data, int (*ptCmpFunc)(long *a, long *b)){
    if (heap_ptr_entity->_heap_num_element_ == heap_ptr_entity->_heap_num_allocated_ ){
        heap_ptr_entity->_heap_num_allocated_ = heap_ptr_entity->_heap_num_allocated_ == 0 ? _heap_init_allocate_num_ : (heap_ptr_entity->_heap_num_allocated_)*2;
        struct heap_data **_tmp = realloc(heap_ptr_entity->_heap_array_, ((heap_ptr_entity->_heap_num_allocated_)*sizeof(struct heap_data *)));
        if (!_tmp){
            printf("[ERR] Couldn't re-allocate memory for the heap, exiting... \n");
            return (0);
        }
        heap_ptr_entity->_heap_array_ = _tmp;
    }
    (heap_ptr_entity->_heap_array_)[heap_ptr_entity->_heap_num_element_++] = new_data;
    _heapify_bottom_up_(heap_ptr_entity, ptCmpFunc);
    return 1;
}

struct heap_data * heap_pop(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *a, long *b)){
    if (!heap_ptr_entity->_heap_num_element_) {return NULL;}
    struct heap_data * tmp = (heap_ptr_entity->_heap_array_)[0];
    // fill the hole left on root with the right-bottom-most element
    (heap_ptr_entity->_heap_array_)[0] = (heap_ptr_entity-> _heap_array_)[--(heap_ptr_entity->_heap_num_element_)];
    (heap_ptr_entity->_heap_array_)[heap_ptr_entity->_heap_num_element_] = NULL;   
    _heapify_top_down_(heap_ptr_entity, ptCmpFunc);
    return tmp;
}

void _heapify_top_down_(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *a, long *b)){
    int i = 0;
    struct heap_data * tmp;
    int lc, rc, maxc;
    while (1){
        lc = 2*i+1;
        rc = lc+1;
        if (lc > heap_ptr_entity->_heap_num_element_ - 1){
            break;
        }
        maxc = lc;
        if (rc < heap_ptr_entity->_heap_num_element_ - 1 && ptCmpFunc(&((heap_ptr_entity->_heap_array_)[rc]->cmp), &((heap_ptr_entity->_heap_array_)[lc]->cmp))>0){
            maxc = rc;
        }
        if (ptCmpFunc(&((heap_ptr_entity->_heap_array_)[i]->cmp), &((heap_ptr_entity->_heap_array_)[maxc]->cmp)) >= 0 ){
            break;
        } else {
            tmp = (heap_ptr_entity->_heap_array_)[i];
            (heap_ptr_entity->_heap_array_)[i] = (heap_ptr_entity->_heap_array_)[maxc];
            (heap_ptr_entity->_heap_array_)[maxc] = tmp;
            i = maxc;
        }
    }
}

void _heapify_bottom_up_(struct heap_entity * heap_ptr_entity, int (*ptCmpFunc)(long *a, long *b)){
    int parent, child;
    struct heap_data *tmp;
    child = heap_ptr_entity->_heap_num_element_ - 1;
    parent = (child-1) / 2;

    while(parent >= 0 && ptCmpFunc(&((heap_ptr_entity->_heap_array_)[child]->cmp), &((heap_ptr_entity->_heap_array_)[parent]->cmp)) > 0){
        tmp = (heap_ptr_entity->_heap_array_)[child];
        (heap_ptr_entity->_heap_array_)[child] = (heap_ptr_entity->_heap_array_)[parent];
        (heap_ptr_entity->_heap_array_)[parent] = tmp;
        child = parent;
        parent = (child-1)/2;
    } 
}

void heap_free(struct heap_entity * heap_ptr_entity){
    free(heap_ptr_entity->_heap_array_);
    heap_ptr_entity->_heap_array_ = NULL;
    heap_ptr_entity->_heap_num_element_ = 0;
    heap_ptr_entity->_heap_num_allocated_ = 0;
}

int heap_is_empty(struct heap_entity * heap_ptr_entity){
    if (heap_ptr_entity->_heap_num_element_ == 0){
        return 1;
    }else{
        return 0;
    }
}
