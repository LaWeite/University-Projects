#include <fstream>
#include <chrono>
#include <iostream>

static void quickSelect(int* array, int left, int right, int k1, int k2) {
    if (left >= right)
    {
        return;
    }

    int l = left;
    int r = right;
    int pivot = array[l + (r - l) / 2];

    while (l <= r)
    {
        while (array[l] < pivot)
        {
            ++l;
        }
        while (array[r] > pivot)
        {
            --r;
        }
        if (l >= r)
        {
            break;
        }
        if (l < r)
        {
            std::swap(array[l], array[r]);
            ++l;
            --r;
        }
    }

    if (k1 <= r) {
        quickSelect(array, left, r, k1, k2);
    }
    if (k2 >= r) {
        quickSelect(array, r + 1, right, k1, k2);
    }
}

int main(int argc, char* argv[]) {
    auto begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        return 1;
    }

    std::ifstream fileInput(argv[1]);
    if (!fileInput.is_open())
    {
        return 1;
    }

    std::ofstream outputStream(argv[2]);
    if (!outputStream.is_open())
    {
        return 1;
    }

    int n;
    int k1;
    int k2;
    int A;
    int B;
    int C;
    int x1;
    int x2;
    fileInput >> n >> k1 >> k2 >> A >> B >> C >> x1 >> x2;

    fileInput.close();

    int* array = new int[n];
    array[0] = x1;
    array[1] = x2;
    for (int i = 2; i < n; ++i)
    {
        array[i] = A * array[i - 2] + B * array[i - 1] + C;
    }

    quickSelect(array, 0, n - 1, k1 - 1, k2 - 1);

    for (int i = k1 - 1; i < k2; ++i) {
        if (i == k2 - 1)
        {
            outputStream << array[i];
            break;
        }
        outputStream << array[i] << " ";
    }

    outputStream.close();
    delete[] array;

    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}
