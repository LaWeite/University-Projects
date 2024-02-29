class Queue
{
public:
	Queue(int capacity);

	int size() const;
	void addToQueue(int number_of_card);
	int takeFromQueue();
	bool isEmpty() const;

	~Queue() {
		if (mas_ != nullptr)
		{
			delete mas_;
			mas_ = nullptr;
		}
	}

private:
	int* mas_;
	int size_;
	int capacity_;
	int first_element_;
	int last_element_;
};
