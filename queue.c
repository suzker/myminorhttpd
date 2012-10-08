#include "queue.h"

int queue_init(struct queue_entity * ptr_queue_entity, int size){
    ptr_queue_entity->_queue_size_ = size;
    ptr_queue_entity->_queue_array_ = malloc((ptr_queue_entity->_queue_size_) *sizeof(void*));
    if (!(ptr_queue_entity->_queue_array_)){return 1;}
    ptr_queue_entity->_queue_idx_head_ = -1;
    ptr_queue_entity->_queue_idx_last_ = -1;
    ptr_queue_entity->_queue_num_element_ = 0;
    return 0;
}

int queue_enqueue(struct queue_entity * ptr_queue_entity, void * new_data){
    if (ptr_queue_entity->_queue_idx_head_ == -1 && ptr_queue_entity->_queue_idx_last_ == -1){ 
        (ptr_queue_entity->_queue_array_)[ptr_queue_entity->_queue_num_element_] = new_data;
        ptr_queue_entity->_queue_idx_head_ = 0;
        ++(ptr_queue_entity->_queue_num_element_);
        ptr_queue_entity->_queue_idx_last_ = ptr_queue_entity->_queue_idx_head_;
    } else {
        if (ptr_queue_entity->_queue_num_element_ < ptr_queue_entity->_queue_size_){
            if (ptr_queue_entity->_queue_idx_last_ > ptr_queue_entity->_queue_size_ - 1){ // round up
                ptr_queue_entity->_queue_idx_last_ = 0;
            } else {
                ++(ptr_queue_entity->_queue_idx_last_);
            }
            (ptr_queue_entity->_queue_array_)[ptr_queue_entity->_queue_idx_last_] = new_data; 
            ++(ptr_queue_entity->_queue_num_element_);
        } else {
            return -1; //queue full
        }
    }
    return 0;
}

void * queue_dequeue(struct queue_entity * ptr_queue_entity){
    if (ptr_queue_entity->_queue_num_element_ > 0){
        void * head_data = (ptr_queue_entity->_queue_array_)[ptr_queue_entity->_queue_idx_head_];
        // increase the pointer to the next queue element;
        if (ptr_queue_entity->_queue_idx_head_ == ptr_queue_entity->_queue_idx_last_){ // if the last element popped out, start over
            ptr_queue_entity->_queue_idx_head_ = -1;
            ptr_queue_entity->_queue_idx_last_ = -1;
        } else {
            if (ptr_queue_entity->_queue_idx_head_ + 1 > ptr_queue_entity->_queue_size_ - 1){ // roundup
                ptr_queue_entity->_queue_idx_head_ = 0;
            } else {
                ++(ptr_queue_entity->_queue_idx_head_);
            }
        }
        --(ptr_queue_entity->_queue_num_element_);
        return head_data;
    }
    return NULL;
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
