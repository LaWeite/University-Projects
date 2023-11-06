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
	bool check = false; //проверяем, что хоть один вход был
	for (int index = 0; index < str.length(); ++index) //идем циклом по всей строчке
	{
		std::string result; //будем записывать сюда подходящую цепочку
		int ind = index; //заводим еще один индекс, чтобы итерироваться по строке
		int state = 0; //состояние
		while (state != -1) //пока не перешли в пустое состояние
		{
			if (str[ind] == '~' || str[ind] == 'a' || str[ind] == 'b') //проверяем на нужные символы, иначе ловим else
			{
				state = table[state][static_cast<int>(str[ind]) % 3]; //задаем состояние
				result += str[ind]; //добавляем текущий элемент к цепочке
				++ind; //увеличиваем ind, двигаясь по строчке
				if (state == 6) //проверяем, что получили может правильную
				{
					std::cout << index + 1 << ": " << result << std::endl; //выводим индекс первого символа и саму цепочку
					check = true; //хоть одна цепочка есть
				}
			}
			else {
				state = -1; //переходим в пустое состояние
			}
		}
	}

	//проверка, что могли не найти ни одной цепочки
	if (!check) {
		std::cout << "not found" << std::endl;
	}
}

int main() {
	std::string input; //заводим переменную для нашей строки

	std::cout << "Write your string:" << std::endl; //просим пользователя ввести строку
	std::cin >> input; //принимаем строку

	find(input); //передаем строку на обработку

	return 0;
}
