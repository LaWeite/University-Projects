#include <iostream>
#include <fstream>
#include "Queue.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		return 1;
	}

	std::fstream fileInput(argv[1]);
	if (!fileInput.is_open())
	{
		return 1;
	}

	Queue first_player(52);
	Queue second_player(52);

	while (fileInput.peek() != -1)
	{
		int number_of_card = 0;
		char string[6]{};

		fileInput.getline(string, 6, '\n');

		if (string[0] >= '1' && string[0] <= '9')
		{
			number_of_card = string[0] == '1' ? 10 : string[0] - '0';
		}
		else {
			if (string[0] == 'J')
			{
				number_of_card = 11;
			}
			else if (string[0] == 'Q') {
				number_of_card = 12;
			} 
			else if (string[0] == 'K') {
				number_of_card = 13;
			}
			else {
				number_of_card = 14;
			}
		}

		if (first_player.size() < 26)
		{
			first_player.addToQueue(number_of_card);
		}
		else {
			second_player.addToQueue(number_of_card);
		}
	}

	Queue queue_during_game(52);
	int count_of_steps = 0;

	while (!first_player.isEmpty() && !second_player.isEmpty())
	{
		if (count_of_steps > 1000000)
		{
			std::cout << "unknown";

			fileInput.close();
			return 0;
		}

		int top_of_first_player = first_player.takeFromQueue();
		int top_of_second_player = second_player.takeFromQueue();

		queue_during_game.addToQueue(top_of_first_player);
		queue_during_game.addToQueue(top_of_second_player);
		
		int size_of_queue_during_game = queue_during_game.size();
		if (top_of_first_player > top_of_second_player)
		{
			if (top_of_first_player == 14 && top_of_second_player == 2) {
				for (int i = 0; i < size_of_queue_during_game; ++i) {
					second_player.addToQueue(queue_during_game.takeFromQueue());
				}
			}
			else {
				for (int i = 0; i < size_of_queue_during_game; ++i) {
					first_player.addToQueue(queue_during_game.takeFromQueue());
				}
			}
		}
		else if (top_of_first_player < top_of_second_player) {
			if (top_of_first_player == 2 && top_of_second_player == 14)
			{
				for (int i = 0; i < size_of_queue_during_game; ++i) {
					first_player.addToQueue(queue_during_game.takeFromQueue());
				}
			}
			else {
				for (int i = 0; i < size_of_queue_during_game; ++i)
				{
					second_player.addToQueue(queue_during_game.takeFromQueue());
				}
			}
		}

		++count_of_steps;
	}

	if (first_player.isEmpty() && second_player.isEmpty())
	{
		std::cout << "draw";
	}
	else if (second_player.isEmpty()) {
		std::cout << "first";
	}
	else {
		std::cout << "second";
	}

	fileInput.close();

	return 0;
}