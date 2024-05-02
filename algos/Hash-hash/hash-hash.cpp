#include <iostream>
#include <fstream>

#include "hash-hash.h";

int main(int argc, char* argv[]) {
	if (argc != 3)
	{
		std::cerr << "error in argc count" << std::endl;
		return 1;
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		std::cerr << "error of open inputFile" << std::endl;
		return 1;
	}

	std::ofstream outputFile(argv[2]);
	if (!outputFile.is_open())
	{
		std::cerr << "error of open outputFile" << std::endl;
		return 1;
	}

	HashTable hashTable;

	int amountN;
	inputFile >> amountN;
	for (int i = 0; i < amountN; ++i)
	{
		char operation;
		int key;
		inputFile >> operation >> key;

		if (operation == '+')
		{
			hashTable.insert(key);
		}
		else if (operation == '-') {
			hashTable.erase(key);
		}
		else if (operation == '?') {
			if (hashTable.find(key))
			{
				outputFile << "true" << "\n";
			}
			else {
				outputFile << "false" << "\n";
			}
		}
	}

	inputFile.close();
	outputFile.close();

	return 0;
}
