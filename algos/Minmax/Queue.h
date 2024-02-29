#include "Stack.h"

class Queue {
private:
    Stack first_stack_;
    Stack second_stack_;
public:
    Queue(int capacity);
    void push(int newElement);
    void pop();
    int difference() const;
};