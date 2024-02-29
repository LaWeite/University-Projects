#include "Queue.h"

Queue::Queue(int capacity) {
    first_stack_.resize(capacity);
    second_stack_.resize(capacity);
}

void Queue::push(int newElement) {
    first_stack_.push(newElement);
}

void Queue::pop() {
    if (first_stack_.size() == 1 && second_stack_.isEmpty())
    {
        first_stack_.pop();
    }

    if (second_stack_.isEmpty()) {
        while (!first_stack_.isEmpty()) {
            second_stack_.push(first_stack_.pop());
        }
    }

    second_stack_.pop();
}

int Queue::difference() const {
    return (first_stack_.maxElement() < second_stack_.maxElement() ? second_stack_.maxElement() : first_stack_.maxElement()) - (first_stack_.minElement() < second_stack_.minElement() ? first_stack_.minElement() : second_stack_.minElement());
}