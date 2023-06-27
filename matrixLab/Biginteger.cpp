#include "Biginteger.h"

std::string plus(const std::string str_1, const std::string str_2) {
	std::string aNew = str_1;
	std::string bNew = str_2;

	if (aNew.size() < bNew.size())
		swap(aNew, bNew);

	int j = aNew.size() - 1;
	for (int i = bNew.size() - 1; i >= 0; --i, --j)
		aNew[j] += (bNew[i] - '0');

	for (int i = aNew.size() - 1; i > 0; --i)
	{
		if (aNew[i] > '9')
		{
			int d = aNew[i] - '0';
			aNew[i - 1] = ((aNew[i - 1] - '0') + d / 10) + '0';
			aNew[i] = (d % 10) + '0';
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
        num_copy /= discharge;
    }

    number_ = new unsigned int[size_];

    for (int i = 0; i < size_; ++i) {
        number_[i] = num % discharge;
        num /= discharge;
    }
}

unsigned int binary_to_num(const std::string num, int size) {
	unsigned int res = 0;
	long long pow = 1;
	for (int i = size - 1; i >= 0; --i) {
		res += (num[i] - '0') * pow;
		pow *= 2;
	}
	return res;
}

std::string del_by_number(const std::string str, const int number) {
	int size = str.length();
	int buffer = str[0] - '0';
	std::string result = "";
	for (int i = 0; i < size; ++i) {
		if (buffer < number && i != size - 1) {
			if (str[i + 1] != '0') {
				buffer = (str[i + 1] - '0') + buffer * 10;
			}
			else {
				buffer *= 10;
			}
			result += '0';
		}
		else {
			result += buffer / number + '0';
			buffer = buffer % number;
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

	if (answer == "0" || answer == "") {
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
		if (i != 0 && std::isdigit(str[i]) == 0) {
			throw std::invalid_argument("Invalid character");
		}
	}

	std::string str_copy = "";
	if (str[0] == '-') {
		sign_ = false;
		str_copy = str.substr(1, str.size() - 1);
	}
	else {
		sign_ = true;
		str_copy = str;
	}


	std::string num = "";
	do {
		num = char((str_copy[str_copy.length() - 1] - '0') % 2 + '0') + num;
		str_copy = del_by_number(str_copy,2);
	} while (str_copy != "0");

	int num_size = num.length();
	size_ = num_size / 32;
	if (num_size % 32 != 0) {
		++size_;
	}

	number_ = new unsigned int[size_];
	std::string buffer = "";
	for (int i = num_size, k = size_ - 1; i > 0; i -= 32, --k) {
		if (i < 32) {
			buffer = num.substr(0, i);
		}
		else {
			buffer = num.substr(i - 32, 32);
		}
		number_[k] = binary_to_num(buffer, buffer.length());
	}

	if (number_[0] == 0)
	{
		sign_ = true;
	}
}

BigInteger::BigInteger(const BigInteger& other) {
    sign_ = other.sign_;
    size_ = other.size_;
    number_ = new unsigned int[size_];
    for (int i = 0; i < size_; ++i) {
        number_[i] = other.number_[i];
    }
}

BigInteger::BigInteger(BigInteger&& other) noexcept
    : sign_(other.sign_), size_(other.size_), number_(other.number_) {
    other.number_ = nullptr;
    other.sign_ = false;
    other.size_ = 0;
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        sign_ = other.sign_;
        number_ = new unsigned int[other.size_];
        for (int i = 0; i < other.size_; ++i)
        {
            number_[i] = other.number_[i];
        }
        size_ = other.size_;
    }
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this != &other) {
        sign_ = other.sign_;
        number_ = other.number_;
        size_ = other.size_;
        other.sign_ = false;
        other.number_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

bool operator==(BigInteger left, BigInteger right) {
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
        return sign_ > other.sign_;
    }
    if (size_ != other.size_) {
        return size_ > other.size_ && sign_ == true ||
            size_ < other.size_&& sign_ == false;
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
    BigInteger result;
    if (sign_ == other.sign_) {
        if (size_ < other.size_) {
            result.size_ = other.size_;
            delete[] result.number_;
            result.number_ = new unsigned int[result.size_];
            int tmp = 0;
            for (int i = size_ - 1, k = other.size_ - 1; i >= 0; --i, --k) {
                unsigned long long plus = static_cast<unsigned long long>(number_[i]) + static_cast<unsigned long long>(other.number_[k]) + tmp;
                result.number_[k] = plus % discharge;
                tmp = plus / discharge;
            }
            for (int i = other.size_ - size_ - 1; i >= 0; --i) {
                unsigned long long plus = static_cast<unsigned long long>(other.number_[i]) + tmp;
                result.number_[i] = plus % discharge;
                tmp = plus / discharge;
            }
            if (tmp != 0) {
                BigInteger result_2;
                result_2.size_ = other.size_ + 1;
                delete[] result_2.number_;
                result_2.number_ = new unsigned int[result_2.size_];
                for (int i = 1; i <= other.size_; ++i) {
                    result_2.number_[i] = result.number_[i - 1];
                }
                result_2.number_[0] = tmp;
                result_2.sign_ = sign_;
                *this = result_2;
                return *this;
            }
            result.sign_ = sign_;
            *this = result;
            return *this;
        }
        else {
            result.size_ = size_;
            result.number_ = nullptr;
            result.number_ = new unsigned int[result.size_];
            int tmp = 0;
            for (int k = size_ - 1, i = other.size_ - 1; i >= 0; --i, --k) {
                unsigned long long plus = static_cast<unsigned long long>(number_[k]) + static_cast<unsigned long long>(other.number_[i]) + tmp;
                result.number_[k] = plus % discharge;
                tmp = plus / discharge;
            }
            for (int i = size_ - other.size_ - 1; i >= 0; --i) {
                unsigned long long plus = static_cast<unsigned long long>(number_[i]) + tmp;
                result.number_[i] = plus % discharge;
                tmp = plus / discharge;
            }
            if (tmp != 0) {
                BigInteger result_2;
                result_2.size_ = size_ + 1;
                delete[] result_2.number_;
                result_2.number_ = new unsigned int[result_2.size_];
                for (int i = 1; i <= size_; ++i) {
                    result_2.number_[i] = result.number_[i - 1];
                }
                result_2.number_[0] = tmp;
                result_2.sign_ = sign_;
                *this = result_2;
                return *this;
            }
            result.sign_ = sign_;
            *this = result;
            return *this;
        }
    }
    else {
        if (*this < -other && sign_ || *this > -other && !sign_) {
            //sign = !sign_, other - this;
            result.size_ = other.size_;
            delete[] result.number_;
            result.number_ = new unsigned int[result.size_];
            bool flag = 0;
            for (int i = other.size_ - 1, k = size_ - 1; k >= 0; --k, --i) {
                unsigned long long minus = 0;
                if (other.number_[i] != 0 && flag) {
                    other.number_[i] -= 1;
                    flag = 0;
                }
                else if (other.number_[i] == 0 && flag) {
                    other.number_[i] = 4294967295;
                }
                if (other.number_[i] < number_[k]) {
                    minus = discharge;
                    flag = 1;
                }
                result.number_[i] = static_cast<unsigned int>(minus + static_cast<unsigned long long>(other.number_[i]) - static_cast<unsigned long long>(number_[k]));
            }
            for (int i = other.size_ - size_ - 1; i >= 0; --i) {
                if (flag && other.number_[i] == 0) {
                    result.number_[i] = 4294967295;
                }
                else if (flag && other.number_[i] != 0) {
                    result.number_[i] = other.number_[i] - 1;
                    flag = 0;
                }
                else {
                    result.number_[i] = other.number_[i];
                }
            }
            int i = 0;
            while (result.number_[i] == 0 && i < result.size_) {
                ++i;
            }
            if (i == result.size_) {
                *this = 0;
                return *this;
            }
            size_ = result.size_ - i;
            delete[] number_;
            number_ = new int unsigned[size_];
            for (int k = 0; k < size_; ++k) {
                number_[k] = result.number_[k + i];
            }
            sign_ = !sign_;
            return *this;
        }
        else {
            //sign = sign_, this - other;  
            result.size_ = size_;
            delete[] result.number_;
            result.number_ = new unsigned int[result.size_];
            for (int i = 0; i < result.size_; ++i) {
                result.number_[i] = 0;
            }
            bool flag = 0;
            for (int i = size_ - 1, k = other.size_ - 1; k >= 0; --k, --i) {
                unsigned long long minus = 0;
                if (number_[i] != 0 && flag) {
                    number_[i] -= 1;
                    flag = 0;
                }
                else if (number_[i] == 0 && flag) {
                    number_[i] = 4294967295;
                }
                if (number_[i] < other.number_[k]) {
                    minus = discharge;
                    flag = 1;
                }
                result.number_[i] = static_cast<unsigned int>(minus + static_cast<unsigned long long>(number_[i]) - static_cast<unsigned long long>(other.number_[k]));
            }
            for (int i = size_ - other.size_ - 1; i >= 0; --i) {
                if (flag && number_[i] == 0) {
                    result.number_[i] = 4294967295;
                }
                else if (flag && number_[i] != 0) {
                    result.number_[i] = number_[i] - 1;
                    flag = 0;
                }
                else {
                    result.number_[i] = number_[i];
                }
            }
            int i = 0;
            while (result.number_[i] == 0 && i < result.size_) {
                ++i;
            }
            if (i == result.size_) {
                *this = 0;
                return *this;
            }
            size_ = result.size_ - i;
            delete[] number_;
            number_ = new int unsigned[size_];
            for (int k = 0; k < size_; ++k) {
                number_[k] = result.number_[k + i];
            }
            return *this;
        }
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    return *this += -other;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    unsigned long long* numbers = new unsigned long long[size_ + other.size_];
    for (int i = 0; i < size_ + other.size_; ++i) {
        numbers[i] = 0;
    }
    unsigned long long product;
    int p1;
    int p2;
    unsigned long long sum;

    for (int i = size_ - 1; i >= 0; --i) {
        for (int k = other.size_ - 1; k >= 0; --k) {
            product = static_cast<unsigned long long>(number_[i]) * other.number_[k];
            p1 = i + k;
            p2 = i + k + 1;

            sum = product + numbers[p2];

            numbers[p1] += sum / discharge;
            numbers[p2] = sum % discharge;
        }
    }

    int j = 0;
    while (numbers[j] == 0 && j < size_ + other.size_) {
        ++j;
    }
    if (j == size_ + other.size_) {
        *this = 0;
        return *this;
    }
    delete[] number_;
    number_ = new unsigned int[size_ + other.size_ - j];
    sign_ = sign_ == other.sign_;
    size_ = size_ + other.size_ - j;
    for (int i = 0; i < size_; ++i) {
        number_[i] = numbers[i + j];
    }
    
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    BigInteger result;
    result.size_ = size_;
    result.number_ = new unsigned int[size_];
    for (int i = 0; i < size_; ++i) {
        result.number_[i] = 0;
    }
    unsigned int left = 0;
    unsigned int right = 4294967295;
    unsigned int half = (right + left) / 2;
    BigInteger result_copy = result;
    for (int i = 0; i < size_; ++i) {
        left = 0;
        right = 4294967295;
        if (i != 0) {
            result_copy.number_[i - 1] = half;
        }
        half = (right + left) / 2;
        result.number_[i] = half;
        result_copy.number_[i] = half + 1;

        while (!(abs(result * other) < abs(*this) && abs(result_copy * other) > abs(*this))) {
            if (abs(result * other) < abs(*this)) {
                left = half;
                half = static_cast<unsigned int>((static_cast<unsigned long long>(right) + left) / 2);
            }
            else if (abs(result * other) > abs(*this)) {
                right = half;
                half = static_cast<unsigned int>((static_cast<unsigned long long>(right) + left) / 2);
            }
            else {
                break;
            }
            result.number_[i] = half;
            result_copy.number_[i] = half + 1;

        }
    }
    int j = 0;
    while (result.number_[j] == 0 && j < size_) {
        ++j;
    }
    if (j == size_) {
        *this = 0;
        return *this;
    }
    delete[] number_;
    number_ = new unsigned int[size_ - j];
    sign_ = sign_ == other.sign_;
    size_ = size_ - j;
    for (int i = 0; i < size_; ++i) {
        number_[i] = result.number_[i + j];
    }

    return *this;

}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this - *this / other * other;
    return *this;
}

BigInteger operator+(BigInteger left, BigInteger right) {
    return left += right;
}

BigInteger operator-(BigInteger left, BigInteger right) {
    return left -= right;
}

BigInteger operator*(BigInteger left, BigInteger right) {
    return left *= right;
}

BigInteger operator/(BigInteger left, BigInteger right) {
    return left /= right;
}

BigInteger operator%(BigInteger left, BigInteger right) {
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
		else
			++v1it;
	}
	while (v1.front() == '0') v1.erase(0, 1);
	if (v1.empty()) v1 = "0";
	return v1;
}

std::string operator<<(std::string str, int s)
{
	for (auto i = s; i != 0; --i) str.push_back('0');
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
		if (carry) ret_.insert(0, 1, carry + '0');
		result = add(result, (ret_ << shift++));
	}
	return result;
}

std::string to_binary(unsigned int num) {
    std::string res = "";
    while (num != 0) {
        res = std::to_string(num % 2) + res;
        num /= 2;
    }
    while (res.size() != 32) {
        res = "0" + res;
    }
    return res;
}

std::string to_string(const BigInteger& num) {
    std::string result_bin = "";
    for (int i = 0; i < num.size_; ++i) {
        result_bin += to_binary(num.number_[i]);
    }
    std::string pow = "1";
    std::string result = "0";
    for (int i = result_bin.size() - 1; i >= 0; --i) {
        if (result_bin[i] == '1') {
            result = plus(result, pow);
        }
        pow = multipy_two(pow);
    }
    if (num.sign_ == 0 && num != 0) {
        result = "-" + result;
    }
    return result;
}

