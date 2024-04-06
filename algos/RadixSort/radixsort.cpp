#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_CHARS = 256;

void countSort(char** arr, int size, int k) {
    int count[MAX_CHARS] = {0};
    char** output = new char*[size];

    for (int i = 0; i < size; ++i) {
        count[(unsigned char)arr[i][k]]++;
    }

    for (int i = 1; i < MAX_CHARS; ++i) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; --i) {
        output[count[(unsigned char)arr[i][k]] - 1] = arr[i];
        count[(unsigned char)arr[i][k]]--;
    }

    for (int i = 0; i < size; ++i) {
        arr[i] = output[i];
    }

    delete[] output;
}

// Применяем LSD сортировку от последнего до (M-K)-го символа
void lsdSort(char** arr, int N, int M, int K) {
    for (int i = M - 1; i >= M - K; --i) {
        countSort(arr, N, i);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <inputfile> <outputfile>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    int N, M, K;
    input >> N >> M >> K;

    // Создаем массив строк
    char** arr = new char*[N];
    for (int i = 0; i < N; ++i) {
        arr[i] = new char[M + 1];  // +1 для нуль-терминатора
    }

    input.ignore(); // Пропускаем перевод строки после чтения чисел

    // Считываем данные в массив в виде столбцов
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            arr[j][i] = input.get();
        }
        input.ignore(); // Пропускаем перевод строки в конце каждой строки
    }

    // Добавляем нуль-терминаторы
    for (int i = 0; i < N; ++i) {
        arr[i][M] = '\0';
    }

    // Выполняем LSD сортировку
    lsdSort(arr, N, M, K);

    // Записываем первые символы отсортированных строк в выходной файл
    for (int i = 0; i < N; ++i) {
        output << arr[i][0];
    }

    // Освобождаем память
    for (int i = 0; i < N; ++i) {
        delete[] arr[i];
    }
    delete[] arr;

    input.close();
    output.close();

    return 0;
}
