#include "queue.h"

int enqueue(void * new_data){
    if (_queue_num_element_ == _queue_num_allocated_){
        _queue_num_allocated_ = _queue_num_allocated_ == 0 ? _queue_init_allocate_num_ : _queue_num_allocated_*2;
        void ** _tmp = realloc(_queue_array_, (_queue_num_allocated_*sizeof(void *)));
        if (!_tmp){
            printf("[ERR] Couldn't re-allocate memory for the queue, exiting... \n");
            return (-1);
        }
        _queue_array_ = _tmp;
    }
    _queue_array_[_queue_num_element_++]  = new_data;
    return 0;
}

void * dequeue(){
    if (_queue_num_element_ > 0){
        void * head_data = _queue_array_[0];
        // increase the pointer to the next queue element;
        _queue_array_ = --_queue_num_allocated_ > 0 ? _queue_array_ = &(_queue_array_[1]) : NULL;
        --_queue_num_element_;
        return head_data;
    }
    return NULL;
}

void queue_free(){
    free(_queue_array_);
    _queue_array_ = NULL;
    _queue_num_allocated_ = 0;
    _queue_num_element_ = 0;
}
