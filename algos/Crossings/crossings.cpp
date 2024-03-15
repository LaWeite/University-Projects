#include <iostream>
#include <fstream>

struct Element {
    int x0;
    int x1;
};

static unsigned long long merge(Element* array, int left, int mid, int right, bool flag) {
    int i = left;
    int j = mid;
    int k = 0;
    unsigned long long count = 0;
    Element* temp = new Element[(right - left + 1)];

    while (i < mid && j <= right) {
        if (flag) {
            if (array[i].x0 <= array[j].x0) {
                if (array[i].x0 == array[j].x0) {
                    count += static_cast<long long>(mid) - i;
                }
                temp[k] = array[i];
                ++k;
                ++i;
            }
            else {
                temp[k] = array[j];
                ++k;
                ++j;
            }
        }
        else {
            if (array[i].x1 < array[j].x1) {
                temp[k] = array[i];
                ++k;
                ++i;
            }
            else {
                count += static_cast<long long>(mid) - i;
                temp[k] = array[j];
                ++k;
                ++j;
            }
        }
    }

    while (i < mid && k < right - left + 1) {
        temp[k] = array[i];
        ++k;
        ++i;
    }

    while (j <= right && k < right - left  + 1) {
        temp[k] = array[j];
        ++k;
        ++j;
    }

    for (i = left, k = 0; i <= right; ++i, ++k) {
        array[i] = temp[k];
    }

    return count;
}

static unsigned long long mergeSort(Element* array, int left, int right, bool flag) {
    if (right == left) {
        return 0;
    }

    unsigned long long count = 0;

    int mid = left + (right - left) / 2;

    count = mergeSort(array, left, mid, flag);
    count += mergeSort(array, mid + 1, right, flag);
    count += merge(array, left, mid + 1, right, flag);

    return count;
}

static unsigned long long getInversions(Element* arr, int n, bool flag) {
    return mergeSort(arr, 0, n - 1, flag);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        return 1;
    }

    int n;
    inputFile >> n;
    Element* array = new Element[n];
    for (int i = 0; i < n; ++i) {
        inputFile >> array[i].x0 >> array[i].x1;
    }

    inputFile.close();

    unsigned long long count = getInversions(array, n, true);

    count += static_cast<unsigned long long>(array[0].x0 == array[n - 1].x0 ? 1 : 0);
    
    count += getInversions(array, n, false);

    std::cout << count << std::endl;

    delete[] array;

    return 0;
}
