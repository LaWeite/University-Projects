struct Number {
    int number_;
    int minimum_ = 2147483647;
    int maximum_ = -2147483647;
};

class Stack {
private:
    Number* data_;
    int topIndex_;
    int capacity_;
public:
    Stack();

    void resize(int capacity);

    void push(int newElement);
    int pop();
    int maxElement() const;
    int minElement() const;
    bool isEmpty() const;
    int size() const;

    ~Stack() {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
    }
};
