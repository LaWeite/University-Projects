#include "Queue.h"

Queue::Queue(int capacity) : size_(0), capacity_(capacity), mas_(nullptr), first_element_(0), last_element_(-1) {
	if (capacity_ > 0)
	{
		mas_ = new int[capacity_];
	}
}

int Queue::size() const
{
	return size_;
}

void Queue::addToQueue(int number_of_card)
{
	++size_;
	last_element_ = (last_element_ + 1) % capacity_;
	mas_[last_element_] = number_of_card;
}

int Queue::takeFromQueue()
{
	--size_;
	int need_number = mas_[first_element_];
	first_element_ = (first_element_ + 1) % capacity_;
	return need_number;
}

bool Queue::isEmpty() const
{
	return size_ == 0;
}