#include <fstream>
#include <iostream>

// Функция для обмена двух элементов
static void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Будем использовать разбиение Hoare
static int hoarePartition(int* arr, int left, int right) {
    int pivot = arr[left];
    int i = left - 1;
    int j = right + 1;
    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);
        do {
            j--;
        } while (arr[j] > pivot);
        if (i >= j) {
            return j;
        }
        swap(arr[i], arr[j]);
    }
}

static void quickSelect(int* arr, int left, int right, int k1, int k2) {
    if (left >= right) return;

    int pivotIndex = hoarePartition(arr, left, right);

    if (k1 <= pivotIndex) {
        quickSelect(arr, left, pivotIndex, k1, k2);
    }
    if (k2 >= pivotIndex + 1) {
        quickSelect(arr, pivotIndex + 1, right, k1, k2);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3)
    {
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

    int* array = new int[n];
    array[0] = x1;
    array[1] = x2;
    for (int i = 2; i < n; ++i)
    {
        array[i] = A * array[i - 2] + B * array[i - 1] + C;
    }

    // Находим k1-ю до k2-ю порядковые статистики
    quickSelect(array, 0, n - 1, k1 - 1, k2 - 1);

    // Выводим результаты
    for (int i = k1 - 1; i < k2; ++i) {
        if (i == k2 - 1)
        {
            outputStream << array[i];
            break;
        }
        outputStream << array[i] << " ";
    }

    delete[] array;
    return 0;
}
