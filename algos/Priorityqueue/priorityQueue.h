#include <iostream>

struct FibonacciHeapNode {
    int key;
    FibonacciHeapNode* parent;
    FibonacciHeapNode* child;
    FibonacciHeapNode* left;
    FibonacciHeapNode* right;
    bool marked;
    int degree;

    FibonacciHeapNode(int key) : key(key), parent(nullptr), child(nullptr),
        left(this), right(this), marked(false), degree(0) {}
};

class FibonacciHeap {
private:
    FibonacciHeapNode* min;
    int n;

    int customLog2(int n) {
        int result = 0;
        while (n > 1) {
            n >>= 1;
            ++result;
        }
        return result;
    }

    int customFloor(double x) {
        int result = (int)x;
        if (x < 0 && x != result) {
            --result;
        }
        return result;
    }

    void consolidate() {
        int max_degree = static_cast<int>(customFloor(customLog2(n))) + 2;
        FibonacciHeapNode** degree_array = new FibonacciHeapNode * [max_degree];

        for (int i = 0; i < max_degree; ++i) {
            degree_array[i] = nullptr;
        }

        FibonacciHeapNode* start = min;
        FibonacciHeapNode* current = min;
        int deg = 0;

        do
        {
            deg = current->degree;
            if (degree_array[deg] != nullptr) {
                FibonacciHeapNode* y = degree_array[deg];

                if (current->key > y->key) {
                    std::swap(current, y);
                }

                link(y, current);
                degree_array[deg] = nullptr;
                continue;
            }

            degree_array[deg] = current;

            current = current->right;

        } while (current != start && degree_array[current->degree] != current);

        min = nullptr;
        for (int i = 0; i < max_degree; ++i) {
            if (degree_array[i] != nullptr) {
                if (min == nullptr || degree_array[i]->key < min->key) {
                    min = degree_array[i];
                }
            }
        }

        delete[] degree_array;
    }

    void link(FibonacciHeapNode* y, FibonacciHeapNode* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        if (x->child != nullptr) {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        else {
            x->child = y;
            y->right = y;
            y->left = y;
        }
        y->parent = x;
        x->degree++;
        y->marked = false;
    }

public:
    FibonacciHeap() : min(nullptr), n(0) {}

    FibonacciHeapNode* insert(int key) {
        FibonacciHeapNode* newNode = new FibonacciHeapNode(key);

        if (min != nullptr) {
            newNode->left = min;
            newNode->right = min->right;
            min->right = newNode;
            newNode->right->left = newNode;
            if (newNode->key < min->key) {
                min = newNode;
            }
        }
        else {
            min = newNode;
        }

        ++n;

        return newNode;
    }

    int extractMin() {
        if (min == nullptr) {
            return -2147483646;
        }

        FibonacciHeapNode* oldMin = min;
        int minKey = min->key;

        if (oldMin->child != nullptr) {
            FibonacciHeapNode* currChild = oldMin->child;
            do {
                FibonacciHeapNode* nextChild = currChild->right;
                min->left->right = currChild;
                currChild->left = min->left;
                min->left = currChild;
                currChild->right = min;
                currChild->parent = nullptr;
                currChild = nextChild;
            } while (currChild != oldMin->child);
        }

        oldMin->left->right = oldMin->right;
        oldMin->right->left = oldMin->left;

        if (oldMin == oldMin->right) {
            min = nullptr;
        }
        else {
            min = oldMin->right;
            consolidate();
        }

        --n;
        delete oldMin;
        return minKey;
    }

    void decreaseKey(FibonacciHeapNode* x, int newKey) {
        if (x == nullptr) {
            return;
        }

        if (newKey > x->key) {
            throw std::invalid_argument("New key is greater than current key");
        }

        x->key = newKey;
        FibonacciHeapNode* y = x->parent;

        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }

        if (x->key < min->key) {
            min = x;
        }
    }

    void cut(FibonacciHeapNode* x, FibonacciHeapNode* y) {
        if (x->right != x) {
            x->right->left = x->left;
            x->left->right = x->right;
        }

        if (y->child == x) {
            if (x->right != x) {
                y->child = x->right;
            }
            else {
                y->child = nullptr;
            }
        }

        y->degree--;

        x->left = min;
        x->right = min->right;
        min->right = x;
        x->right->left = x;

        x->parent = nullptr;
        x->marked = false;
    }

    void cascadingCut(FibonacciHeapNode* y) {
        FibonacciHeapNode* z = y->parent;
        if (z != nullptr) {
            if (!y->marked) {
                y->marked = true;
            }
            else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }
};
