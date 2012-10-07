#include "heap.h"

int add_to_heap(struct heap_data * new_data, int (*ptCmpFunc)(long *a, long *b)){
    _heap_num_allocated_ = _heap_num_allocated_ == 0 ? _heap_init_allocate_num_ : _heap_num_allocated_*2;
    struct heap_data **_tmp = realloc(_heap_array_, (_heap_num_allocated_*sizeof(struct heap_data)));
    if (!_tmp){
        printf("[ERR] Couldn't re-allocate memory for the heap, exiting... \n");
        return (-1);
    }
    _heap_array_ = _tmp;
    _heap_array_[_heap_num_element_++] = new_data;
    _heapify_bottom_top_(ptCmpFunc);
    return 0;
}

struct heap_data * heap_pop(int (*ptCmpFunc)(long *a, long *b)){
    if (!_heap_num_element_) {return NULL;}
    struct heap_data * tmp = _heap_array_[0];
    // fill the hole left on root with the right-bottom-most element
    _heap_array_[0] = _heap_array_[--_heap_num_element_];
    _heap_array_[_heap_num_element_ - 1] = NULL;
    _heapify_top_down_(ptCmpFunc);
}

void _heapify_top_down_(int (*ptCmpFunc)(long *a, long *b)){
    int i = 0;
    struct heap_data * tmp;
    int lc, rc, maxc;
        while (1){
        lc = 2*i+1;
        rc = lc+1;
        if (lc > _heap_num_element_){
            break;
        }
        maxc = lc;
        if (rc < _heap_num_element_ && ptCmpFunc(&(_heap_array_[rc]->cmp), &(_heap_array_[lc]->cmp))>0){
            maxc = rc;
        }
        if (ptCmpFunc(&(_heap_array_[i]->cmp), &(_heap_array_[maxc]->cmp)) >= 0 ){
            break;
        } else {
            tmp = _heap_array_[i];
            _heap_array_[i] = _heap_array_[maxc];
            _heap_array_[maxc] = tmp;
            i = maxc;
        }
    }
}

void _heapify_bottom_up_(int (*ptCmpFunc)(long *a, long *b)){
    int parent, child;
    struct heap_data *tmp;
    child = _heap_num_element_ - 1;
    parent = (child-1) / 2;

    while(parent >= 0 && ptCmpFunc(&(_heap_array_[child]->cmp), &(_heap_array_[parent]->cmp)) > 0){
        tmp = _heap_array_[child];
        _heap_array_[child] = _heap_array_[parent];
        _heap_array_[parent] = tmp;
        child = parent;
        parent = (child-1)/2;
    } 
}
