#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>

int heapSize = 0;

static void siftDown(int start, int* heap) {
	int root = start;
	while (root * 2 + 1 < heapSize) {
		int child = root * 2 + 1;
		int swapIdx = root;
		if (heap[swapIdx] < heap[child]) {
			swapIdx = child;
		}
		if (child + 1 < heapSize && heap[swapIdx] < heap[child + 1]) {
			swapIdx = child + 1;
		}
		if (swapIdx == root) {
			return;
		}
		else {
			int temp = heap[root];
			heap[root] = heap[swapIdx];
			heap[swapIdx] = temp;
			root = swapIdx;
		}
	}
}

static void pushHeap(int value, int* heap, int size) {
	if (heapSize >= size) return;
	int index = heapSize++;
	heap[index] = value;
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (heap[parent] < heap[index]) {
			int temp = heap[parent];
			heap[parent] = heap[index];
			heap[index] = temp;
			index = parent;
		}
		else {
			break;
		}
	}
}

static int popHeap(int* heap) {
	if (heapSize <= 0) return -1; // Ошибка, куча пуста
	int result = heap[0];
	heap[0] = heap[--heapSize];
	siftDown(0, heap);
	return result;
}

// Для генерации рекуррентной последовательности
static int generateNext(int A, int B, int C, int x1, int x2) {
	return A * x1 + B * x2 + C;
}

static void quickSort(int arr[], int left, int right)
{
	if (left >= right)
	{
		return;
	}

	int pivotVal = arr[(left + right) / 2];

	int i = left;
	int j = right;
	while (i <= j)
	{
		while (arr[i] < pivotVal)
		{
			++i;
		}
		while (arr[j] > pivotVal)
		{
			--j;
		}
		if (i <= j)
		{
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			++i;
			--j;
		}
	}
	quickSort(arr, left, j);
	quickSort(arr, i, right);
}

int main(int argc, char* argv[])
{
	auto begin = std::chrono::steady_clock::now();

	int k1;
	int k2;
	int n;
	int A;
	int B;
	int C;
	int x1;
	int x2;
	std::ifstream inFile(argv[1]);
	std::ofstream outFile(argv[2]);

	if (!inFile.is_open())
	{
		return -1;
	}
	inFile >> n >> k1 >> k2 >> A >> B >> C >> x1 >> x2;

	int* heap = new int[k2] {0};

	int k1thElement = 0;

	int current;
	int next_x1 = x1;
	int next_x2 = x2;

	if (k1 > 2)
	{
		pushHeap(x1, heap, k2);
		pushHeap(x2, heap, k2);
	}

	for (int i = 3; i <= n; ++i) {
		current = generateNext(A, B, C, next_x1, next_x2);

		if (heapSize < k1) {
			pushHeap(current, heap, k2);
		}
		else if (current < heap[0]) {
			// Если текущий элемент меньше максимального в куче
			popHeap(heap);
			pushHeap(current, heap, k2);
		}

		next_x1 = next_x2;
		next_x2 = current;
	}
	
	// k1-й наименьший элемент будет на вершине кучи после обработки n элементов
	if (heapSize > 0)
	{
		k1thElement = heap[0];
	}

	if (k1 == 1 && k1thElement > x1)
	{
		k1thElement = x1;
	}

	//Необходимо сбросить
	next_x1 = x1;
	next_x2 = x2;
	heapSize = 0;
	for (int i = 3; i <= n; ++i) {
		current = generateNext(A, B, C, next_x1, next_x2);

		if (heapSize < k2) {
			pushHeap(current, heap, k2);
		}
		else if (current < heap[0]) {
			// Если текущий элемент меньше максимального в куче
			popHeap(heap);
			pushHeap(current, heap, k2);
		}

		next_x1 = next_x2;
		next_x2 = current;
	}

	// k2-й наименьший элемент будет на вершине кучи после обработки n элементов
	int k2thElement = 0;
	if (heapSize > 1)
	{
		k2thElement = heap[0];
	}

	int ans[200] = { 0 };
	int count = 0;
	next_x1 = x1;
	next_x2 = x2;

	int count1 = 0;
	int count2 = 0;
	if (x1 == k1thElement)
	{
		ans[count++] = x1;
		count1 = 1;
	}
	else if (x1 == k2thElement)
	{
		ans[count++] = x1;
		count2 = 1;
	}
	
	if (x2 == k2thElement)
	{
		ans[count++] = x2;
		count2 = 1;
	}
	else if (x2 == k1thElement)
	{
		ans[count++] = x2;
		count1 = 1;
	}

	for (int i = 3; i <= n; ++i) {
		current = generateNext(A, B, C, next_x1, next_x2);

		if (k2 - k1 + 1 == count)
		{
			break;
		}

		bool flag = false;
		if (current == k1thElement && count1 < 1)
		{
			ans[count++] = current;
			flag = true;
		}
		else if (count1 > 0) {
			--count1;
		}
		
		if (current == k2thElement && count2 < 1 && !flag)
		{
			ans[count++] = current;
		}
		else if (count2 > 0) {
			--count2;
		}

		if (current > k1thElement && current < k2thElement)
		{
			ans[count++] = current;
		}

		next_x1 = next_x2;
		next_x2 = current;
	}

	quickSort(ans, 0, count - 1);

	for (int i = 0; i < count; ++i)
	{
		outFile << ans[i];
		if (i != count - 1)
		{
			outFile << " ";
		}
	}

	delete[] heap;

	inFile.close();
	outFile.close();

	auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

	return 0;
}
