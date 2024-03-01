#include "Stack.h"

Stack::Stack() : capacity_(0), topIndex_(-1), data_(nullptr) {}

void Stack::resize(int capacity) {
    capacity_ = capacity;
    data_ = new Number[capacity];
}

void Stack::push(int newElement) {
    Number number;
    if (topIndex_ != -1) {
        number = data_[topIndex_];
    }

    number.number_ = newElement;
    number.minimum_ = newElement < number.minimum_ ? newElement : number.minimum_;
    number.maximum_ = newElement > number.maximum_ ? newElement : number.maximum_;
    
    data_[++topIndex_] = number;
}

int Stack::pop() {
    return data_[topIndex_--].number_;
}

int Stack::maxElement() const {
    return topIndex_ != -1 ? data_[topIndex_].maximum_ : -2147483647;
}

int Stack::minElement() const {
    return topIndex_ != -1 ? data_[topIndex_].minimum_ : 2147483647;
}

bool Stack::isEmpty() const {
    return topIndex_ + 1 == 0;
}
