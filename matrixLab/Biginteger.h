#pragma once
#define discharge 4294967296

#include <cstring>
#include <iostream>
#include <string>

class BigInteger {
private:
	bool sign_;
	unsigned int* number_;
	int size_;

public:
	const bool sign() { return sign_; }
	const unsigned int* numbers() { return number_; }
	const int size() { return size_; }
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

	BigInteger abs(const BigInteger& num);

	friend std::string to_string(const BigInteger& num);
};

std::ostream& operator<<(std::ostream& output, BigInteger& other);

BigInteger operator+(const BigInteger left, const BigInteger right);
BigInteger operator-(const BigInteger left, const BigInteger right);
BigInteger operator*(const BigInteger left, const BigInteger right);
BigInteger operator/(const BigInteger left, const BigInteger right);
BigInteger operator%(const BigInteger left, const BigInteger right);

bool operator==(const BigInteger left, const BigInteger right);
