#include "queue.h"

void ** _queue_array_;
int _queue_idx_head_ = -1; // the first element of the queue
int _queue_idx_last_ = -1; // the last element of the queue
int _queue_num_element_ = 0;
int _queue_size_ = 0;

int queue_init(int size){
    _queue_size_ = size;
    _queue_array_ = malloc(_queue_num_element_*sizeof(void*));
    if (!_queue_array_){return 1;}
    _queue_idx_head_ = -1;
    _queue_idx_last_ = -1;
    _queue_num_element_ = 0;
    return 0;
}

int queue_enqueue(void * new_data){
    if (_queue_idx_head_ == -1 && _queue_idx_last_ == -1){ 
        _queue_array_[_queue_num_element_] = new_data;
        _queue_idx_head_ = 0;
        ++_queue_num_element_;
        _queue_idx_last_ = _queue_idx_head_;
    } else {
        if (_queue_num_element_ < _queue_size_){
            if (_queue_idx_last_ > _queue_size_ - 1){ // round up
                _queue_idx_last_ = 0;
            } else {
                ++_queue_idx_last_;
            }
            _queue_array_[_queue_idx_last_] = new_data; 
            ++_queue_num_element_;
        } else {
            return -1; //queue full
        }
    }
    return 0;
}

void * queue_dequeue(){
    if (_queue_num_element_ > 0){
        void * head_data = _queue_array_[_queue_idx_head_];
        // increase the pointer to the next queue element;
        if (_queue_idx_head_ == _queue_idx_last_){ // if the last element popped out, start over
            _queue_idx_head_ = -1;
            _queue_idx_last_ = -1;
        } else {
            if (_queue_idx_head_ + 1 > _queue_size_ - 1){ // roundup
                _queue_idx_head_ = 0;
            } else {
                ++_queue_idx_head_;
            }
        }
        --_queue_num_element_;
        return head_data;
    }
    return NULL;
}

void queue_free(){
    free(_queue_array_);
    _queue_num_element_ = 0;
}

int queue_get_num_elements(){
    return _queue_num_element_;
}
