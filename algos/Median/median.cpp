#include <fstream>
#include <iostream>

void heapify(double* array, int i, int n)
{
    int largest = i;
    int left = 1 + 2 * i;
    int right = 2 + 2 * i;

    if (right < n && array[right] > array[largest])
        largest = right;

    if (left < n && array[left] > array[largest])
        largest = left;

    if (i != largest)
    {
        std::swap(array[i], array[largest]);

        heapify(array, largest, n);
    }
}

void heapSort(double* array, int n)
{
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(array, i, n);

    for (int i = n - 1; i >= 0; --i)
    {
        std::swap(array[0], array[i]);
        heapify(array, 0, i);
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::ifstream fileInput(argv[1]);
    if (!fileInput.is_open()) {
        return 1;
    }

    int amount;
    fileInput >> amount;

    double* startArray = new double[amount];
    double* sortedArray = new double[amount];

    for (int i = 0; i < amount; ++i) {
        fileInput >> sortedArray[i];
        startArray[i] = sortedArray[i];
    }

    heapSort(sortedArray, amount);

    int indexOfMin = 0;
    bool minFind = false;
    int indexOfMid = 0;
    bool midFind = false;
    int indexOfMax = 0;
    bool maxFind = false;
    int index = 0;
    
    while (index != amount) {
        if (startArray[indexOfMin] != sortedArray[0] && !minFind)
        {
            ++indexOfMin;
        }
        else {
            minFind = true;
        }
    
        if (startArray[indexOfMax] != sortedArray[amount - 1] && !midFind)
        {
            ++indexOfMax;
        }
        else {
            midFind = true;
        }
    
        if (startArray[indexOfMid] != sortedArray[amount / 2] && !maxFind)
        {
            ++indexOfMid;
        }
        else {
            maxFind = true;
        }
    
        if (maxFind && minFind && midFind)
        {
            break;
        }
        
        ++index;
    }

    std::cout << indexOfMin + 1 << ' ' << indexOfMid + 1 << ' ' << indexOfMax + 1;

    delete[] startArray;
    delete[] sortedArray;

    fileInput.close();

    return 0;
}
