#include "queue.h"

int queue_init(struct queue_entity * ptr_queue_entity, int size){
    ptr_queue_entity->_queue_size_ = size;
    ptr_queue_entity->_queue_array_ = malloc((ptr_queue_entity->_queue_size_) *sizeof(void*));
    if (!(ptr_queue_entity->_queue_array_)){return 0;}
    ptr_queue_entity->_queue_idx_head_ = -1;
    ptr_queue_entity->_queue_idx_last_ = 0;
    ptr_queue_entity->_queue_num_element_ = 0;
    return 1;
}

int queue_enqueue(struct queue_entity * ptr_queue_entity, void * new_data){
    if (ptr_queue_entity->_queue_num_element_ == ptr_queue_entity->_queue_size_){
        return 0;
    }
    ptr_queue_entity->_queue_array_[ptr_queue_entity->_queue_idx_last_] = new_data;
    if (ptr_queue_entity->_queue_num_element_ == 0){
        ptr_queue_entity->_queue_idx_head_ = ptr_queue_entity->_queue_idx_last_;
    }
    ptr_queue_entity->_queue_idx_last_ = ((++(ptr_queue_entity->_queue_idx_last_))%(ptr_queue_entity->_queue_size_));
    ++(ptr_queue_entity->_queue_num_element_);
    return 1;
}

void * queue_dequeue(struct queue_entity * ptr_queue_entity){
    if (ptr_queue_entity->_queue_num_element_ <= 0){
        return NULL;
    }
    void * _tmp_head_;
    _tmp_head_ = ptr_queue_entity->_queue_array_[ptr_queue_entity->_queue_idx_head_];
    ptr_queue_entity->_queue_idx_head_ = ((++(ptr_queue_entity->_queue_idx_head_))%(ptr_queue_entity->_queue_size_));
    --(ptr_queue_entity->_queue_num_element_);
    return _tmp_head_;
}

void queue_free(struct queue_entity * ptr_queue_entity){
    free(ptr_queue_entity->_queue_array_);
    ptr_queue_entity->_queue_num_element_ = 0;
}

int queue_is_full(struct queue_entity * ptr_queue_entity){
    if (ptr_queue_entity->_queue_num_element_ == ptr_queue_entity->_queue_size_){
        return 1;
    } else {
        return 0;
    }
}

int queue_is_empty(struct queue_entity * ptr_queue_entity){
    if (ptr_queue_entity->_queue_num_element_ == 0){
        return 1;
    } else {
        return 0;
    }
}
