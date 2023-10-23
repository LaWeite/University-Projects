#include "Biginteger.h"

long long del = static_cast<long long>(UINT32_MAX) + 1;

BigInteger::BigInteger() {
	number_ = new unsigned int[1];
	number_[0] = 0;
	sign_ = true;
	size_ = 1;
}

BigInteger::BigInteger(long long num) : sign_(num >= 0), size_(0) {
	num = std::abs(num);
	long long num_copy = num;

	if (num_copy == 0) {
		++size_;
	}

	while (num_copy > 0) {
		++size_;
		num_copy /= del;
	}

	number_ = new unsigned int[size_];

	for (int i = 0; i < size_; ++i) {
		number_[i] = num % del;
		num /= del;
	}
}

std::string div(const std::string& str) {
	int size = static_cast<int>(str.length());
	int buffer = str[0] - '0';
	std::string result;
	for (int i = 0; i < size; ++i) {
		if (buffer < 2 && i != size - 1) {
			if (str[i + 1] != '0') {
				buffer = (str[i + 1] - '0') + buffer * 10;
			}
			else {
				buffer *= 10;
			}
			result += '0';
		}
		else {
			result += static_cast<char>(buffer / 2);
			buffer = buffer % 2;
			if (i != size - 1) {
				buffer = str[i + 1] - '0' + buffer * 10;
			}
		}
	}

	int i = 0;
	while (result[i] == '0' && i < result.length() - 1) {
		++i;
	}

	if (i == result.length())
	{
		return "0";
	}

	std::string answer = result.substr(i);

	if (answer == "0" || !answer.empty()) {
		return "0";
	}

	return answer;
}

BigInteger::BigInteger(const std::string& str) {
	if (str.empty()) {
		throw std::invalid_argument("Empty string");
	}

	if (str[0] == '-' && str[1] == '\0') {
		throw std::invalid_argument("Invalid character");
	}

	for (int i = 0; i < str.size(); ++i) {
		if (str[0] == '-')
		{
			continue;
		}

		if (std::isdigit(str[i]) == 0) {
			throw std::invalid_argument("Invalid character");
		}
	}

	std::string str_copy;
	if (str[0] == '-') {
		sign_ = false;
		for (size_t i = 1; i < str.length(); ++i)
		{
			str_copy[i] = str[i];
		}
	}
	else {
		sign_ = true;
		str_copy = str;
	}


	std::string number;
	do {
		char str_last_check = str_copy[str_copy.length() - 1];
		int last_div = (str_last_check - '0') % 2;
		number = char(last_div + '0') + number;
		str_copy = div(str_copy);
	} while (str_copy != "0");

	size_ = static_cast<int>(number.length()) / 32;
	if (number.length() % 32 != 0) {
		++size_;
	}

	number_ = new unsigned int[size_];
	std::string buffer;
	for (int i = static_cast<int>(number.length()), k = size_ - 1; i > 0; i -= 32, --k) {
		if (i >= 32) {
			buffer = number.substr(i - 32, 32);
		}
		else {
			buffer = number.substr(0, i);
		}

		int size = static_cast<int>(buffer.length());
		long long pow = 1;
		number_[k] = 0;
		for (int i = size - 1; i >= 0; --i) {
			number_[k] += (buffer[i] - '0') * pow;
			pow *= 2;
		}
	}

	if (size_ > 1 && number_[0] == 0)
	{
		sign_ = true;
	}
}

BigInteger::BigInteger(const BigInteger& other) : size_(other.size_), sign_(other.sign_) {
	this->number_ = new unsigned int[this->size_];
	for (int index = 0; index < this->size_; ++index) {
		this->number_[index] = other.number_[index];
	}
}

BigInteger::BigInteger(BigInteger&& other) noexcept {
	this->swap(other);
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
	if (this != &other) {
		BigInteger tmp(other);
		this->swap(tmp);
	}

	return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
	if (this != &other) {
		this->swap(other);
	}
	return *this;
}

bool operator==(BigInteger const& left, BigInteger const& right) {
	if (left.sign() != right.sign()) {
		return false;
	}
	if (left.size() != right.size()) {
		return false;
	}
	int i = 0;
	while (i < left.size()) {
		if (left.numbers()[i] - right.numbers()[i] != 0) {
			return false;
		}

		++i;
	}
	return true;
}

bool BigInteger::operator!=(const BigInteger& other) const {
	return !(*this == other);
}

bool BigInteger::operator>(const BigInteger& other) const {
	if (sign_ != other.sign_) {
		return static_cast<int>(sign_) > static_cast<int>(other.sign_);
	}
	if (size_ != other.size_) {
		return size_ > other.size_ && sign_ == static_cast<bool>(1) ||
			size_ < other.size_ && sign_ == static_cast<bool>(0);
	}
	int i = 0;
	while (i < size_) {
		if (number_[i] - other.number_[i] != 0) {
			return sign_ ? number_[i] > other.number_[i] :  number_[i] < other.number_[i];
		}
		++i;
	}
	return false;
}

bool BigInteger::operator>=(const BigInteger& other) const {
	return *this > other || *this == other;
}

bool BigInteger::operator<(const BigInteger& other) const {
	return !(*this >= other);
}

bool BigInteger::operator<=(const BigInteger& other) const {
	return !(*this > other);
}

BigInteger BigInteger::operator-() const {
	BigInteger result(*this);
	if (*this != 0) {
		result.sign_ = !result.sign_;
	}
	return result;
}

BigInteger BigInteger::operator+() const {
	return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
	BigInteger ans;
	delete[] ans.number_;
	if (sign_ == other.sign_)
	{
		unsigned long long temp = 0;
		bool check_size = false;
		ans.size_ = (size_ > other.size_) ? size_ : other.size_;
		check_size = (size_ > other.size_);
		ans.number_ = new unsigned int[ans.size_];
		int i;
		int k;
		i = (check_size) ? other.size_ - 1 : size_ - 1;
		k = (check_size) ? size_ - 1 : other.size_ - 1;
		for (; i >= 0; --i, --k) {
			unsigned long long plus = (check_size) ? static_cast<unsigned long long>(number_[k]) + static_cast<unsigned long long>(other.number_[i]) + temp : static_cast<unsigned long long>(number_[i]) + static_cast<unsigned long long>(other.number_[k]) + temp;
			ans.number_[k] = plus % del;
			temp = plus / del;
		}
		int variant = (check_size) ? (size_ - other.size_ - 1) : (other.size_ - size_ - 1);
		for (int i = variant; i >= 0; --i) {
			unsigned long long plus = temp;
			if (check_size)
			{
				plus += static_cast<unsigned long long>(number_[i]);
			}
			else {
				plus += static_cast<unsigned long long>(other.number_[i]);
			}
			ans.number_[i] = plus % del;
			temp = plus / del;
		}
		if (temp == 0) {
			ans.sign_ = sign_;
			*this = ans;
			return *this;
		}

		size_ = (check_size) ? size_ + 1 : other.size_ + 1;
		delete[] number_;
		number_ = new unsigned int[size_];
		for (int i = 1; i <= size_ - 1; ++i) {
			number_[i] = ans.number_[i - 1];
		}
		number_[0] = temp;
		return *this;

	}

	bool various_cases = (*this < -other && sign_ || *this > -other && !sign_);
	ans.size_ = various_cases ? other.size_ : size_;
	ans.number_ = new unsigned int[ans.size_];
	bool flag_check = false;

	int i = (various_cases) ? other.size_ - 1 : size_ - 1;
	int k = (various_cases) ? size_ - 1 : other.size_ - 1;

	for (; k >= 0; --k, --i) {
		unsigned long long minus = 0;
		if (various_cases)
		{
			if (flag_check)
			{
				if (other.number_[i] != 0) {
					other.number_[i] -= 1;
					flag_check = false;
				}
				else if (other.number_[i] == 0) {
					other.number_[i] = 4294967295;
				}
			}
			if (other.number_[i] < number_[k]) {
				minus = del;
				flag_check = true;
			}
		}
		else {
			if (flag_check)
			{
				if (number_[i] != 0) {
					number_[i] -= 1;
					flag_check = false;
				}
				else if (number_[i] == 0) {
					number_[i] = 4294967295;
				}
			}
			if (number_[i] < other.number_[k]) {
				minus = del;
				flag_check = true;
			}
		}
		if (various_cases)
		{
			ans.number_[i] = static_cast<unsigned int>(minus + static_cast<unsigned long long>(other.number_[i]) - static_cast<unsigned long long>(number_[k]));
		}
		else {
			ans.number_[i] = static_cast<unsigned int>(minus + static_cast<unsigned long long>(number_[i]) - static_cast<unsigned long long>(other.number_[k]));
		}
	}

	int m = (various_cases) ? other.size_ - size_ - 1 : size_ - other.size_ - 1;
	for (; m >= 0; --m) {
		if (various_cases)
		{
			if (flag_check && other.number_[m] == 0) {
				ans.number_[m] = 4294967295;
			}
			else if (flag_check && other.number_[m] != 0) {
				ans.number_[m] = other.number_[m] - 1;
				flag_check = false;
			}
			else {
				ans.number_[m] = other.number_[m];
			}
		}
		else {
			if (flag_check && number_[m] == 0) {
				ans.number_[m] = 4294967295;
			}
			else if (flag_check && number_[m] != 0) {
				ans.number_[m] = number_[m] - 1;
				flag_check = false;
			}
			else {
				ans.number_[m] = number_[m];
			}
		}
	}

	int zero = 0;
	while (zero < ans.size_ && ans.number_[zero] == 0) {
		++zero;
	}

	if (zero == ans.size_) {
		*this = 0;
		return *this;
	}

	size_ = ans.size_ - zero;
	if (various_cases)
	{
		sign_ = !sign_;
	}
	delete[] number_;
	number_ = new int unsigned[size_];
	for (int k = 0; k < size_; ++k) {
		number_[k] = ans.number_[k + zero];
	}

	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
	return *this += -other;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
	long int aLength = size_;
	long int bLength = other.size_;
	long int newSize = aLength + bLength;

	unsigned long long* numbers = new unsigned long long[newSize];
	for (int i = 0; i < newSize; ++i) {
		numbers[i] = 0;
	}

	unsigned long long sum;
	int point_1 = 0;
	int point_2 = 0;
	for (int i = static_cast<int>(aLength) - 1; i >= 0; --i) {
		for (int j = static_cast<int>(bLength) - 1; j >= 0; --j) {
			point_1 = point_2 = j + i;
			sum = (static_cast<unsigned long long>(number_[i]) * other.number_[j]) + numbers[point_2 + 1];

			numbers[point_1] += sum / del;
			numbers[point_2 + 1] = sum % del;
		}
	}

	int zero = 0;
	while (zero < newSize && numbers[zero] == 0) {
		zero++;
	}

	if (zero == size_ + other.size_) {
		*this = 0;
		delete[] numbers;
		return *this;
	}

	delete[] number_;
	size_ = static_cast<int>(newSize) - zero;
	if (sign_ != other.sign_)
	{
		sign_ = false;
	}
	else {
		if (sign_ == static_cast<bool>(0) && other.sign_ == static_cast<bool>(0))
		{
			sign_ = true;
		}
	}
	number_ = new unsigned int[size_];
	for (int n = 0; n < size_; ++n) {
		number_[n] = numbers[n + zero];
	}

	delete[] numbers;

	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
	BigInteger answer;
	answer.size_ = size_;
	delete[] answer.number_;
	answer.number_ = new unsigned int[size_];
	for (int i = 0; i < size_; ++i) {
		answer.number_[i] = 0;
	}

	unsigned long low = 0;
	unsigned long high = 4294967295;
	unsigned long mid = (low + high) / 2;

	BigInteger answer_copy = answer;
	for (int i = 0; i < size_; ++i) {
		low = 0;
		high = 4294967295;
		if (i != 0) {
			answer_copy.number_[i - 1] = mid;
		}
		mid = (high + low) / 2;
		answer.number_[i] = mid;
		answer_copy.number_[i] = mid + 1;

		BigInteger copy_1 = abs(answer * other);
		BigInteger copy_2 = abs(answer_copy * other);
		while (!(copy_1 < abs(*this) && copy_2 > abs(*this)))
		{
			if (copy_1 < abs(*this) || copy_1 > abs(*this))
			{
				if (copy_1 < abs(*this))
				{
					low = mid;
				}
				else if (copy_1 > abs(*this))
				{
					high = mid;
				}
				mid = static_cast<unsigned int>((static_cast<unsigned long long>(high) + low) / 2);
			}
			else {
				break;
			}

			answer.number_[i] = mid;
			answer_copy.number_[i] = mid + 1;
			copy_1 = abs(answer * other);
			copy_2 = abs(answer_copy * other);
		}
	}

	int zero = 0;
	while (zero < size_ && answer.number_[zero] == 0) {
		++zero;
	}
	if (zero == size_) {
		*this = 0;
		return *this;
	}
	delete[] number_; //mas

	size_ = size_ - zero;
	number_ = new unsigned int[size_]; //mas
	if (sign_ != other.sign_)
	{
		sign_ = false;
	}
	else {
		if (sign_ == static_cast<bool>(0) && other.sign_ == static_cast<bool>(0))
		{
			sign_ = true;
		}
	}

	for (int i = 0; i < size_; ++i) {
		number_[i] = answer.number_[i + zero];
	}

	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
	*this = *this - *this / other * other;
	return *this;
}

BigInteger operator+(BigInteger left, BigInteger const& right) {
	return left += right;
}

BigInteger operator-(BigInteger left, BigInteger const& right) {
	return left -= right;
}

BigInteger operator*(BigInteger left, BigInteger const& right) {
	return left *= right;
}

BigInteger operator/(BigInteger left, BigInteger const& right) {
	return left /= right;
}

BigInteger operator%(BigInteger left, BigInteger const& right) {
	return left %= right;
}

BigInteger BigInteger::abs(const BigInteger& num) {
	BigInteger result = num;
	result.sign_ = true;
	return result;
}

BigInteger& BigInteger::operator++() {
	return *this += 1;
}

BigInteger& BigInteger::operator--() {
	return *this -= 1;
}

BigInteger BigInteger::operator++(int) {
	BigInteger result = *this;
	++(*this);
	return result;
}

BigInteger BigInteger::operator--(int) {
	BigInteger result = *this;
	--(*this);
	return result;
}

std::string to_binary(unsigned int num) {
	std::string res;
	while (num != 0) {
		res.insert(0, std::to_string(num % 2));
		num /= 2;
	}
	while (res.length() != 32) {
		res.insert(0, "0");
	}

	return res;
}

std::string add(std::string v1, std::string v2)
{
	const auto v1size = v1.size();
	const auto v2size = v2.size();
	const auto size = std::max(v1size, v2size) + 1;
	v1.insert(0, size - v1size, '0');
	v2.insert(0, size - v2size, '0');
	auto v1it = v1.rbegin();
	for (auto v2it = v2.rbegin(); v2it != v2.rend(); ++v2it)
	{
		*v1it += *v2it - '0';
		if (*v1it - '0' >= 10)
		{
			*v1it -= 10;
			++(*(++v1it));
		}
		else {
			++v1it;
		}
	}

	while (v1.front() == '0') {
		v1.erase(0, 1);
	}

	if (v1.empty()) {
		v1 = "0";
	}

	return v1;
}

std::string operator<<(std::string str, int s)
{
	for (auto i = s; i != 0; --i) {
		str.push_back('0');
	}

	return str;
}

std::string multipy_two(std::string a) {
	std::string b = "2";
	std::string result;
	auto shift = 0;
	for (auto itb = b.rbegin(); itb != b.rend(); ++itb)
	{
		auto carry = 0;
		std::string ret_;
		for (auto ita = a.rbegin(); ita != a.rend(); ++ita)
		{
			auto p = (*itb - '0') * (*ita - '0') + carry;
			ret_.insert(0, 1, (p % 10) + '0');
			carry = p / 10;
		}
		if (static_cast<bool>(carry)) {
			ret_.insert(0, 1, carry + '0');
		}
		result = add(result, (ret_ << shift++));
	}
	return result;
}

std::string plus(const std::string& str_1, const std::string& str_2) {
	std::string aNew = str_1;
	std::string bNew = str_2;

	if (aNew.size() < bNew.size()) {
		swap(aNew, bNew);
	}

	int j = static_cast<int>(aNew.size()) - 1;
	for (int i = static_cast<int>(bNew.size()) - 1; i >= 0; --i, --j) {
		aNew[j] += (bNew[i] - '0');
	}

	for (int i = static_cast<int>(aNew.size()) - 1; i > 0; --i)
	{
		if (aNew[i] > '9')
		{
			int d = aNew[i] - '0';
			aNew[i - 1] = ((aNew[i - 1] - '0') + d / 10) + '0';
			aNew[i] = static_cast<char>(d % 10 + '0');
		}
	}

	if (aNew[0] > '9')
	{
		std::string temp;
		temp += aNew[0];
		aNew[0] = ((aNew[0] - '0') % 10) + '0';
		temp[0] = ((temp[0] - '0') / 10) + '0';
		aNew = temp + aNew;
	}

	return aNew;
}

std::string to_string(const BigInteger& num) {
	std::string result_bin;
	std::string temp;
	for (int i = 0; i < num.size_; ++i) {
		temp = to_binary(num.number_[i]);
		result_bin += temp;
	}
	std::string pow = "1";
	std::string result = "0";
	for (int i = static_cast<int>(result_bin.size()) - 1; i >= 0; --i) {
		if (result_bin[i] == '1') {
			result = plus(result, pow);
		}
		pow = multipy_two(pow);
	}
	if (static_cast<int>(num.sign_) == 0 && num != 0) {
		result = "-" + result;
	}
	return result;
}
