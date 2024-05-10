#include <iostream>
#include <fstream>

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

    void consolidate() {
        int maxDegree = 64;
        FibonacciHeapNode** degrees = new FibonacciHeapNode * [maxDegree];
        for (int i = 0; i < maxDegree; ++i) {
            degrees[i] = nullptr;
        }
        FibonacciHeapNode* first = min;
        FibonacciHeapNode* x = min;
        while (true) {
            int deg = x->degree;
            if (degrees[deg] != nullptr) {
                FibonacciHeapNode* y = degrees[deg];
                if (y == x) {
                    break;
                }
                if (x == first) {
                    break;
                }
                if (x->key > y->key) {
                    FibonacciHeapNode* tmp = x;
                    x = y;
                    y = tmp;
                }
                link(y, x);
                degrees[deg] = nullptr;
                ++deg;
                continue;
            }
            degrees[deg] = x;
            ++deg;

            x = x->right;
        }

        min = nullptr;
        for (int i = 0; i < maxDegree; ++i) {
            if (degrees[i] != nullptr) {
                if (min == nullptr || degrees[i]->key < min->key) {
                    min = degrees[i];
                }
            }
        }
        delete[] degrees;
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
        // Создаем новый узел
        FibonacciHeapNode* newNode = new FibonacciHeapNode(key);

        // Вставляем узел в список корней
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

        n++;

        return newNode;
    }

    int extractMin() {
        if (min == nullptr) {
            return -2147483646; // Куча пуста.
        }

        FibonacciHeapNode* oldMin = min;
        int minKey = min->key;

        if (oldMin->child != nullptr) {
            FibonacciHeapNode* currChild = oldMin->child;
            do {
                FibonacciHeapNode* nextChild = currChild->right;
                // Добавить детей удалённого минимума в список корней
                min->left->right = currChild;
                currChild->left = min->left;
                min->left = currChild;
                currChild->right = min;
                currChild->parent = nullptr;
                currChild = nextChild;
            } while (currChild != oldMin->child);
        }

        // Удаляем min из списка корней
        oldMin->left->right = oldMin->right;
        oldMin->right->left = oldMin->left;

        if (oldMin == oldMin->right) {
            min = nullptr;
        }
        else {
            min = oldMin->right;
            consolidate();
        }

        n--;
        delete oldMin;
        return minKey;
    }

    void decreaseKey(FibonacciHeapNode* x, int newKey) {
        if (x == nullptr) return; // Узел не существует
        if (newKey > x->key) {
            // Новый ключ больше текущего ключа, что нарушает свойства кучи
            throw std::invalid_argument("New key is greater than current key");
        }

        x->key = newKey;
        FibonacciHeapNode* y = x->parent;

        if (y != nullptr && x->key < y->key) {
            // Вырезаем узел x и помещаем его в список корней
            cut(x, y);
            cascadingCut(y);
        }

        if (x->key < min->key) {
            min = x;
        }
    }

    void cut(FibonacciHeapNode* x, FibonacciHeapNode* y) {
        // Удаляем x из списка дочерних элементов y
        if (x->right != x) {
            x->right->left = x->left;
            x->left->right = x->right;
        }

        if (y->child == x) {
            // Если у y есть другие дети
            if (x->right != x) {
                y->child = x->right;
            }
            else {
                y->child = nullptr;
            }
        }

        y->degree--;

        // Добавляем x в список корней кучи
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
