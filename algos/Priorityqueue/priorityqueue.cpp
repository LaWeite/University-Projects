#include <iostream>
#include <fstream>

#include "priorityQueue.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    std::ifstream fin(argv[1]);
    if (!fin.is_open()) {
        return 1;
    }

    std::ofstream fout(argv[2]);
    if (!fout.is_open()) {
        return 1;
    }

    char str[1024];
    int n = 0;
    while (fin.peek() != EOF) {
        fin.getline(str, 1024);
        ++n;
    }

    fin.clear();
    fin.seekg(0);

    FibonacciHeapNode** arr = new FibonacciHeapNode * [n];
    FibonacciHeap queue;

    for (int i = 0; i < n; ++i) {
        fin.getline(str, 1024);
        int idx = 0, x = 0, y = 0, e = 0;
        bool minus = false;

        if (str[0] == 'p') {
            idx = 5;
            if (str[idx] == '-') {
                minus = true;
                ++idx;
            }
            while (str[idx] >= '0' && str[idx] <= '9') {
                x = x * 10 + (str[idx] - '0');
                ++idx;
            }
            if (minus) x = -x;
            arr[i] = queue.insert(x);
        }
        else if (str[0] == 'e') {
            e = queue.extractMin();
            if (e == -2147483646) {
                fout << "*\n";
            }
            else {
                fout << e << '\n';
            }
        }
        else {
            idx = 13;
            if (str[idx] == '-') {
                minus = true;
                ++idx;
            }
            while (str[idx] != ' ') {
                x = x * 10 + (str[idx] - '0');
                ++idx;
            }
            if (minus) x = -x;
            ++idx;
            minus = false;
            if (str[idx] == '-') {
                minus = true;
                ++idx;
            }
            while (str[idx] != '\0') {
                y = y * 10 + (str[idx] - '0');
                ++idx;
            }
            if (minus) y = -y;
            queue.decreaseKey(arr[x - 1], y);
        }
    }

    delete[] arr;
    fin.close();
    fout.close();
    return 0;
}
