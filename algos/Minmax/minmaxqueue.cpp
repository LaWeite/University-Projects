#include <iostream>﻿
#include <fstream>

#include "Queue.h"

int main(int argc, char* argv[]) {
    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        return 1;
    }

    int sizeCommands = 0;

    inputFile >> sizeCommands;

    Queue queue(sizeCommands);

    char command = 0;
    int number = 0;
    for (int i = 0; i < sizeCommands; ++i)
    {
        inputFile >> command;

        if (command == '+')
        {
            inputFile >> number;
            queue.push(number);
        }
        else if (command == '-')
        {
            queue.pop();
        } 
        else if (command == '?')
        {
            outputFile << queue.difference() << std::endl;
        }
    }

    outputFile.close();
    inputFile.close();

    return 0;
}