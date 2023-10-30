#include "database.h"

template <typename T>
T* increaseArraySize(T* array, int size) {
	T* newArray = new T[size];

	for (int i = 0; i < size - 1; ++i) {
		newArray[i] = array[i];
	}

	delete[] array;
	return newArray;
}

void copy(application& temp, const char* date, const char* time, const char* ticker, const char* type) {
	for (int i = 0; i < 10; ++i) {
		temp.date_[i] = date[i];
		if (i + 1 == 10)
		{
			temp.date_[10] = '\0';
		}
	}

	for (int i = 0; i < 8; ++i) {
		temp.time_[i] = time[i];
		if (i + 1 == 8)
		{
			temp.time_[8] = '\0';
		}
	}

	for (int i = 0; i < 15; ++i) {
		temp.ticker_[i] = ticker[i];
		if (i + 1 == 15)
		{
			temp.ticker_[15] = '\0';
		}
	}

	if (compareWords(type, "buy") == 0) {
		temp.buy = static_cast<bool>(1);
	}
	else {
		temp.buy = false;
	}
}

void add(database& DB, const char* date, const char* time, const char* ticker, const char* type, const char* amount, const char* price, const bool sort) {
	++DB.count;
	if (DB.count > DB.maxsize)
	{
		DB.maxsize = (DB.maxsize != 0) ? 2 * DB.maxsize : 1;
		application* temp = new application[DB.maxsize];
		for (int i = 0; i < DB.count - 1; ++i)
		{
			temp[i] = DB.data[i];
		}

		delete[] DB.data;
		DB.data = temp;
	}

	application temp;
	temp.id_ = DB.count;
	copy(temp, date, time, ticker, type);

	int amount_temp = std::atoi(amount);
	double price_temp = strtod(price, NULL);

	if (DB.count > 1 && sort) {
		int* index_of_same_ticker = NULL;
		int size = 0;

		for (int i = 0; i < DB.count - 1; ++i)
		{
			if (compareWords(ticker, DB.data[i].ticker_) != 0)
			{
				continue;
			}

			if (DB.data[i].amount_ != 0 && ((!temp.buy && DB.data[i].buy) || (temp.buy && !DB.data[i].buy)))
			{
				if (size == 0)
				{
					++size;
					delete[] index_of_same_ticker;
					index_of_same_ticker = new int[size];
					index_of_same_ticker[size - 1] = i;
					continue;
				}

				++size;
				index_of_same_ticker = increaseArraySize(index_of_same_ticker, size);
				index_of_same_ticker[size - 1] = i;
			}
		}

		for (int i = 0; i < size - 1; ++i) {
			for (int j = 0; j < size - 1 - i; ++j) {
				if (DB.data[index_of_same_ticker[j]].price_ > DB.data[index_of_same_ticker[j + 1]].price_) {
					std::swap(index_of_same_ticker[j], index_of_same_ticker[j + 1]);
				}
			}
		}

		if (size > 0)
		{
			if (!temp.buy)
			{
				for (int i = size - 1; i >= 0; --i)
				{
					if (price_temp <= DB.data[index_of_same_ticker[i]].price_) {
						if (amount_temp == 0) {
							break;
						}

						if (amount_temp - DB.data[index_of_same_ticker[i]].amount_ <= 0) {
							DB.data[index_of_same_ticker[i]].amount_ -= amount_temp;
							amount_temp = 0;
						}
						else {
							amount_temp -= DB.data[index_of_same_ticker[i]].amount_;
							DB.data[index_of_same_ticker[i]].amount_ = 0;
						}
					}
				}
			}
			else {
				for (int i = 0; i < size; ++i) {
					if (price_temp >= DB.data[index_of_same_ticker[i]].price_) {
						if (amount_temp == 0) {
							break;
						}

						if (amount_temp - DB.data[index_of_same_ticker[i]].amount_ <= 0) {
							DB.data[index_of_same_ticker[i]].amount_ -= amount_temp;
							amount_temp = 0;
						}
						else {
							amount_temp -= DB.data[index_of_same_ticker[i]].amount_;
							DB.data[index_of_same_ticker[i]].amount_ = 0;
						}
					}
				}
			}

			delete[] index_of_same_ticker;
		}
	}

	temp.amount_ = amount_temp;
	temp.price_ = price_temp;
	DB.data[DB.count - 1] = temp;
}

void print(const database& DB) {
	for (int i = 0; i < DB.count; ++i) {
		std::cout << DB.data[i].id_ << " " << DB.data[i].date_ <<  " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
		if (DB.data[i].buy) {
			std::cout << "buy, ";
		}
		else {
			std::cout << "sell, ";
		}
		std::cout << DB.data[i].amount_ << ", " << DB.data[i].price_ << std::endl;
	}
}

void sortByDate(database& DB, const int start, const int end) {
	for (int i = 0; i < end - start - 1; ++i) {
		for (int j = 0 + start; j < end - 1 - i; ++j) {
			if (strcmp(DB.data[j].date_, DB.data[j + 1].date_) == 0) {
				if (strcmp(DB.data[j].time_, DB.data[j + 1].time_) > 0) {
					std::swap(DB.data[j], DB.data[j + 1]);
				}
				else if (strcmp(DB.data[j].time_, DB.data[j + 1].time_) == 0) {
					if (strcmp(DB.data[j].ticker_, DB.data[j + 1].ticker_) > 0) {
						std::swap(DB.data[j], DB.data[j + 1]);
					}
					else if (strcmp(DB.data[j].ticker_, DB.data[j + 1].ticker_) == 0) {
						if (!DB.data[j].buy && DB.data[j + 1].buy) {
							std::swap(DB.data[j], DB.data[j + 1]);
						}
						else if (!DB.data[j].buy && !DB.data[j + 1].buy) {
							if (DB.data[j].price_ < DB.data[j + 1].price_) {
								std::swap(DB.data[j], DB.data[j + 1]);
							}
						}
						else {
							if (DB.data[j].price_ > DB.data[j + 1].price_) {
								std::swap(DB.data[j], DB.data[j + 1]);
							}
						}
					}
				}
			}
			else if (strcmp(DB.data[j].date_, DB.data[j + 1].date_) > 0) {
				std::swap(DB.data[j], DB.data[j + 1]);
			}
			else {
				return;
			}
		}
	}
}

void sortByBuySell(database& DB, const int start, const int end) {
	for (int i = start; i < end - 1; ++i) {
		for (int j = start; j < end - 1 - i; ++j) {
			if (DB.data[j].buy && !DB.data[j + 1].buy) {
				std::swap(DB.data[j], DB.data[j + 1]);
			}
		}
	}

	for (int i = start; i < end; ++i)
	{
		if (DB.data[i].buy)
		{
			sortByDate(DB, i, end);
			if (i != start)
			{
				sortByDate(DB, start, i);
			}

			break;
		}

		if (i == end - 1)
		{
			sortByDate(DB, start, end);
		}
	}
}

int findRecord(database& DB, const int id) {
	for (int i = 0; i < DB.count; ++i) {
		if (DB.data[i].id_ == id) {
			return i;
		}
	}

	return -1;
}

void deleteRecord(database& DB, const int id) {
	if (DB.data != nullptr) {
		int index = findRecord(DB, id);
		if (index < 0)
		{
			std::cout << "no such record" << std::endl;
			return;
		}

		DB.data[index] = DB.data[DB.count - 1];
		--DB.count;
	}
}

void clear(database& DB) {
	for (int i = 0; i < DB.count; ++i) {
		if (DB.data[i].amount_ == 0) {
			deleteRecord(DB, DB.data[i].id_);
		}
	}
}

void sortByPrice(database& DB) {
	for (int i = 0; i < DB.count - 1; ++i) {
		for (int j = 0; j < DB.count - 1 - i; ++j) {
			if (DB.data[j].price_ < DB.data[j + 1].price_) {
				std::swap(DB.data[j], DB.data[j + 1]);
			}
		}
	}
}

void dom(database& DB, const char* ticker) {
	database domDB = { 0,0,nullptr };
	for (int i = 0; i < DB.count; ++i) {
		if (compareWords(ticker, DB.data[i].ticker_) != 0)
		{
			continue;
		}
		
		if (DB.data[i].amount_ > 0) {
			++domDB.count;
			if (domDB.count > domDB.maxsize) {
				domDB.maxsize = (domDB.maxsize != 0) ? 2 * domDB.maxsize : 1;

				if (domDB.maxsize == 1)
				{
					domDB.data = new application[1];
					domDB.data[domDB.count - 1] = DB.data[i];
					continue;
				}

				domDB.data = increaseArraySize(domDB.data, domDB.maxsize);
			}
			domDB.data[domDB.count - 1] = DB.data[i];
		}
	}

	sortByPrice(domDB);
	
	for (int i = 0; i < domDB.count - 1; ++i)
	{
		if (domDB.data[i].price_ == domDB.data[i+1].price_)
		{
			int temp_begin = i;
			do
			{
				++i;
			} while (domDB.data[i].price_ == domDB.data[i + 1].price_ && i < domDB.count - 1);

			sortByBuySell(domDB, temp_begin, i);
		}
	}

	std::cout << "> > ";
	for (int i = 0; i < domDB.count; ++i) {
		if (domDB.data[i].buy) {
			std::cout << "buy, ";
		}
		else {
			std::cout << "sell, ";
		}
		std::cout << std::fixed << std::setprecision(2) << domDB.data[i].price_ << ", " << domDB.data[i].amount_ << ", ";
		std::cout << domDB.data[i].date_[6] << domDB.data[i].date_[7] << domDB.data[i].date_[8] << domDB.data[i].date_[9] << DB.data[i].date_[5] << domDB.data[i].date_[3] << domDB.data[i].date_[4] << DB.data[i].date_[2] << domDB.data[i].date_[0] << domDB.data[i].date_[1];
		std::cout << " " << domDB.data[i].time_ << std::endl;
	}
	std::cout << "> ";

	delete[] domDB.data;
}
