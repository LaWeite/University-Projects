#include <iostream>
#include <string>

//Таблица переходов		  ~   a   b
static const int table[7][3] = { {1, -1, -1}, //q0
			         {6, 2, -1}, //q1
				 {-1, 3, 4}, //q2
  				 {-1, 2, 5}, //q3
	 			 {-1, -1, 1}, //q4
				 {6, 2, -1}, //q5
				 {-1, -1, -1} }; //*q6

void find(const std::string& str) {
	int strLen = str.length(); //длина исходной строки
	int state = 0; //состояние
	std::string result; //будем записывать сюда подходящую цепочку
	bool check = false; //проверяем, что хоть один вход был

	//идем циклом по всей строчке
	for (int index = 0; index < strLen; ++index)
	{
		switch (str[index]) {
			case 126: //встретили ~
				state = table[state][0];
				break;
			case 97: //встретили a
				//проверяем, что мы не зашли сюда после неправильной последовательности
				/*if (state == 0)
				{
					continue;
				}*/
				state = table[state][1];
				break;
			case 98: //встретили b
				//проверяем, что мы не зашли сюда после неправильной последовательности
				/*if (state == 0)
				{
					continue;
				}*/
				state = table[state][2];
				break;
			default: //если встречаем символ не из алфавита
				result = ""; //обнуляем строчку
				state = 0;//обнуляем состояние
				continue; //переходим на следующий элемент
		}

		//добавляем текущий элемент к цепочке
		result += str[index];

		//если мы перешли в пустое состояние
		if (state == -1)
		{
			result = ""; //обнуляем строку
			state = 0; //обнуляем состояние

			//уменьшаем индекс, только, если может быть началом следующей строки
			if (str[index - 1] == '~')
			{
				--index;
			}
		}

		if (state == 6)
		{
			std::cout << index - result.length() + 2 << ": " << result << std::endl; //выводим индекс первого символа и саму цепочку
			check = true; //хоть одна цепочка есть
			result = ""; //обнуляем строчку, тк завершили цепочку
			state = 0; //обнуляем состояние, тк завершили цепочку
			--index; //возвращаемся на один индекс назад, тк он может быть начало следующей цепочки 
		}
	}

	if (!check) {
		std::cout << "cant find substring!" << std::endl; //не нашли такие цепочки
	}
}

int main() {
	std::string input; //заводим переменную для нашей строки

	std::cout << "Write your string:" << std::endl; //просим пользователя ввести строку
	std::cin >> input; //принимаем строку

	find(input); //передаем строку на обработку

	return 0;
}
