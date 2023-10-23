#include <cstring>
#include <iostream>
#include <string>

class BigInteger {
private:
	bool sign_;
	unsigned int* number_ = nullptr;
	int size_;

	void swap(BigInteger& other) {
		std::swap(this->number_, other.number_);
		std::swap(this->sign_, other.sign_);
		std::swap(this->size_, other.size_);
	}
public:
	bool sign() const { return sign_; }
	unsigned int* numbers() const { return number_; }
	int size() const { return size_; }
	BigInteger();
	BigInteger(long long num);
	explicit BigInteger(const std::string& str);
	BigInteger(const BigInteger& other);
	BigInteger(BigInteger&& other) noexcept;
	BigInteger& operator=(const BigInteger& other);
	BigInteger& operator=(BigInteger&& other) noexcept;

	bool operator!=(const BigInteger& other) const;
	bool operator>(const BigInteger& other) const;
	bool operator>=(const BigInteger& other) const;
	bool operator<(const BigInteger& other) const;
	bool operator<=(const BigInteger& other) const;

	BigInteger operator-() const;
	BigInteger operator+() const;

	BigInteger& operator+=(const BigInteger& other);
	BigInteger& operator-=(const BigInteger& other);
	BigInteger& operator*=(const BigInteger& other);
	BigInteger& operator/=(const BigInteger& other);
	BigInteger& operator%=(const BigInteger& other);

	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger& operator--();
	BigInteger operator--(int);

	static BigInteger abs(const BigInteger& num);

	friend std::string to_string(const BigInteger& num);

	~BigInteger() {
		delete[] number_;
	}
};

std::ostream& operator<<(std::ostream& output, BigInteger& other);

BigInteger operator+(BigInteger left, BigInteger const & right);
BigInteger operator-(BigInteger left, BigInteger const & right);
BigInteger operator*(BigInteger left, BigInteger const & right);
BigInteger operator/(BigInteger left, BigInteger const & right);
BigInteger operator%(BigInteger left, BigInteger const & right);

bool operator==(BigInteger const & left, BigInteger const & right);
