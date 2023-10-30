#include "database.h"

void define(const char* input, char* date, char* time, char* ticker, char* buy_sell, char* amount, char* price, int index = 0) {
	int index_temp = 0;
	for (int i = index; i < 200; ++i)
	{
		while (input[i] != ' ')
		{
			date[index_temp++] = input[i];
			++i;
		}

		++i;
		index_temp = 0;
		while (input[i] != ',')
		{
			time[index_temp++] = input[i];
			++i;
		}

		i += 2;
		index_temp = 0;
		
		while (input[i] != ',')
		{
			ticker[index_temp++] = input[i];
			++i;
		}

		++i;
		index_temp = 0;
		while (input[i] != ',')
		{
			if (input[i] != ' ')
			{
				buy_sell[index_temp++] = input[i];
			}
			++i;
		}

		++i;
		index_temp = 0;
		while (input[i] != ',')
		{
			if (input[i] != ' ')
			{
				amount[index_temp++] = input[i];
			}
			++i;
		}

		++i;
		index_temp = 0;
		while (input[i] != '\0')
		{
			if (input[i] != ' ') {
				price[index_temp++] = input[i];
			}
			++i;
		}

		break;
	}
}

bool check(int year) {
	return (!((!static_cast<bool>((year) % 4) && static_cast<bool>((year) % 100)) || (!static_cast<bool>((year) % 400))));
}

int main(int argc, char* argv[])
{
	char input[200] = "";
	database DB = { 0, 0, nullptr };

	while (std::cin.getline(input, 200)) {
		char command[50] = "";
		char date[50] = "";
		char time[50] = "";
		char ticker[50] = "";
		char buy_sell[50] = "";
		char amount[50] = "";
		char price[50] = "";
		char inp_file[200] = "";
		char out_file[200] = "";
		int index = 0;

		for (; index < 200; ++index) {
			if (input[index] == ' ') {
				command[index] = '\0';
				++index;
				break;
			}
			command[index] = input[index];
		}

		if (compareWords(command, "load") == 0) {
			for (int i = 0; i < 200; ++i, ++index) {
				inp_file[i] = input[index];
			}

			std::ifstream fin(inp_file);

			char symbol;
			char line[200] = "";
			int i = 0;
			while (!fin.eof()) {
				symbol = static_cast<char>(fin.get());

				if (symbol == '\n') {
					define(line, date, time, ticker, buy_sell, amount, price);
					add(DB, date, time, ticker, buy_sell, amount, price, false);
					for (int i = 0; i < 50; ++i)
					{
						date[i] = 0;
						time[i] = 0;
						ticker[i] = 0;
						buy_sell[i] = 0;
						amount[i] = 0;
						price[i] = 0;
					}

					i = 0;

				}
				else {
					line[i++] = symbol;
				}
			}

			fin.close();
		}
		else if (compareWords(command, "save") == 0) {
			for (int i = 0; i < 200; ++i, ++index) {
				out_file[i] = input[index];
			}

			std::ofstream output(out_file);

			for (int i = 0; i < DB.count; ++i) {
				output << DB.data[i].date_ << " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
				if (DB.data[i].buy) {
					output << "buy, ";
				}
				else {
					output << "sell, ";
				}
				output << DB.data[i].amount_ << ", " << std::fixed << std::setprecision(2) << DB.data[i].price_ << std::endl;
			}

			output.close();
		}
		else if (compareWords(command, "add") == 0) {
			define(input, date, time, ticker, buy_sell, amount, price, index);

			if (date[2] != '.' || date[5] != '.') {
				std::cout << "> Wrong date format: " << date << "\n> ";
			}
			else if ((date[3] == '0' && date[4] == '0') || (date[4] > '2') || (date[3] > '1'))
			{
				std::cout << "> Month value is invalid: " << date[2] << date[3] << "\n> ";
			}
			else if (time[2] != ':' || time[5] != ':') {
				std::cout << "> Wrong time format: " << time << "\n> ";
			}
			else if ((date[0] == '3' && date[1] > '1') || (date[0] == '0' && date[1] == '0') || (date[3] == '0' && date[4] == '2' && date[0] == '2' && date[1] == '9' && check(atoi(date))))
			{
				std::cout << "> Day value is invalid: " << date[0] << date[1] << "\n> ";
			}
			else if ((time[0] == '2' && time[1] > '4') || (time[0] > '3') || time[3] > '5' || time[6] > '5') {
				std::cout << "> Time value is invalid: " << time << "\n> ";
			}
			else if (buy_sell[0] != 'b' && buy_sell[0] != 's') {
				std::cout << "> Unknown command: " << buy_sell << "\n> ";
			}
			else {
				add(DB, date, time, ticker, buy_sell, amount, price, true);
			}
		}
		else if (compareWords(command, "delete") == 0) {
			char id_of_t[50] = "";
			for (int i = 0; i < 50; ++i, ++index) {
				if (isdigit(input[index]) == 0) {
					break;
				}
				id_of_t[i] = input[index];
			}

			int id = atoi(id_of_t);
			deleteRecord(DB, id);
		}
		else if (compareWords(command, "clear") == 0) {
			/*for (int i = 0; i < DB.count; ++i) {
				if (DB.data[i].amount_ == 0) {
					deleteRecord(DB, DB.data[i].id_);
				}
			}*/

			int* indexes = new int[DB.count];
			int index_temp = 0;
			for (int i = 0; i < DB.count; ++i) {
				if (DB.data[i].amount_ == 0) {
					indexes[index_temp++] = DB.data[i].id_;
				}
			}

			for (int i = 0; i < index_temp; ++i) {
				deleteRecord(DB, indexes[i]);
			}

			delete[] indexes;
		}
		else if (compareWords(command, "print") == 0) {
			print(DB);
		}
		else if (compareWords(command, "dom") == 0) {
			for (int i = 0; i < 15; ++i, ++index) {
				ticker[i] = input[index];
			}
			dom(DB, ticker);
		}
		else if (compareWords(command, "export") == 0) {
			for (int i = 0; i < 200; ++i, ++index) {
				out_file[i] = input[index];
			}
			std::ofstream out(out_file);

			sortByDate(DB, 0, DB.count);

			for (int i = 0; i < DB.count; ++i) {
				out << DB.data[i].date_[6] << DB.data[i].date_[7] << DB.data[i].date_[8] << DB.data[i].date_[9] << DB.data[i].date_[5];
				out << DB.data[i].date_[3] << DB.data[i].date_[4] << DB.data[i].date_[2];
				out << DB.data[i].date_[0] << DB.data[i].date_[1];
				out << " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
				if (DB.data[i].buy) {
					out << "buy, ";
				}
				else {
					out << "sell, ";
				}
				out << DB.data[i].amount_ << ", " << std::fixed << std::setprecision(2) << DB.data[i].price_ << std::endl;
			}

			out.close();
		}
		else if (compareWords(command, "quit") == 0) {
			delete[] DB.data;
			return 0;
		}
		else {
			std::cout << "> Unknown command: " << command << "\n> ";
		}
	}
}
