#include <iostream>

constexpr auto size_of_table = 100000;

struct Pair {
	int number;
	Pair* next;

	Pair(int number) : number(number), next(nullptr) {};
};

class HashTable
{
public:
	HashTable();

        void insert(int key);
        void erase(int key);
        bool find(int key);

	~HashTable();

private:
	Pair** table;
};

HashTable::HashTable()
{
	table = new Pair * [size_of_table]();
}

HashTable::~HashTable()
{
	for (int i = 0; i < size_of_table; ++i)
	{
		Pair* current = table[i];
		while (current != nullptr) {
			Pair* toDelete = current;
			current = current->next;
			delete toDelete;
		}
	}

	delete[] table;
}

int hash(int key) {
    return std::abs(key) % size_of_table;
}

void HashTable::insert(int key) {
    int id = hash(key);
    Pair* pair = table[id];
    while (pair != nullptr) {
        if (pair->number == key) {
            return;
        }
        pair = pair->next;
    }

    pair = new Pair(key);
    pair->next = table[id];
    table[id] = pair;
}

void HashTable::erase(int key) {
    int id = hash(key);
    Pair* pair = table[id];
    Pair* previous = nullptr;
    while (pair != nullptr) {
        if (pair->number == key) {
            if (previous == nullptr) {
                table[id] = pair->next;
            }
            else {
                previous->next = pair->next;
            }
            delete pair;
            return;
        }
        previous = pair;
        pair = pair->next;
    }
}

bool HashTable::find(int key) {
    int id = hash(key);
    Pair* pair = table[id];
    while (pair != nullptr) {
        if (pair->number == key) {
            return true;
        }
        pair = pair->next;
    }
    return false;
}
