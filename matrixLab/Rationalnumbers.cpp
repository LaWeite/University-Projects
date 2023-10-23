#include "Rationalnumbers.h"

BigInteger greatest_common_divisor(BigInteger left, BigInteger right) {
	if (left % right == 0) {
		return right;
	}
	if (right % left == 0) {
		return left;
	}
	if (left > right) {
		return greatest_common_divisor(left % right, right);
	}
	return greatest_common_divisor(left, right % left);
}

void Rational::reduction() {
	bool flag = false;
	if (numerator_ < 0)
	{
		numerator_ = -numerator_;
		flag = true;
	}
	BigInteger nod = greatest_common_divisor(numerator_, denominator_);

	numerator_ /= nod;
	denominator_ /= nod;

	if (flag)
	{
		numerator_ = -numerator_;
	}
}

Rational::Rational() : numerator_(0), denominator_(1) {}

Rational::Rational(int i) : numerator_(i), denominator_(1) {}

Rational::Rational(BigInteger numerator, BigInteger denomenator) : numerator_(std::move(numerator)), denominator_(std::move(denomenator)) {}

Rational::Rational(const Rational& other) {
	numerator_ = other.numerator_;
	denominator_ = other.denominator_;
}

Rational::Rational(Rational&& other) noexcept {
	this->swap(other);
}

Rational& Rational::operator=(const Rational& other) {
	if (this != &other) {
		Rational tmp(other);
		this->swap(tmp);
	}

	return *this;
}

Rational& Rational::operator=(Rational&& other) noexcept {
	if (this != &other) {
		this->swap(other);
	}
	return *this;
}

bool operator==(Rational left, Rational right) {
	return (left.numerator() == right.numerator() && left.denominator() == right.denominator());
}

bool Rational::operator!=(const Rational& other) const {
	return !(*this == other);
}

bool Rational::operator>(const Rational& other) const {
	if ((numerator_ == other.numerator_) && (denominator_ == other.denominator_))
	{
		return false;
	}

	if (denominator_ == other.denominator_)
	{
		return false ? numerator_ < other.numerator_ : numerator_ > other.numerator_;
	}

	if (numerator_ == other.numerator_)
	{
		return false ? denominator_ > other.denominator_ : denominator_ < other.denominator_;
	}

	if ((numerator_ * other.denominator_) == (other.numerator_ * denominator_))
	{
		return false;
	}

	if ((numerator_ * other.denominator_) <= (other.numerator_ * denominator_))
	{
		return false;
	}

	return true;
}

bool Rational::operator>=(const Rational& other) const {
	return *this > other || *this == other;
}

bool Rational::operator<(const Rational& other) const {
	if ((numerator_ == other.numerator_) && (denominator_ == other.denominator_))
	{
		return false;
	}

	if (denominator_ == other.denominator_)
	{
		return false ? numerator_ > other.numerator_ : numerator_ < other.numerator_;
	}

	if (numerator_ == other.numerator_)
	{
		return false ? denominator_ < other.denominator_ : denominator_ > other.denominator_;
	}

	if ((numerator_ * other.denominator_) == (other.numerator_ * denominator_))
	{
		return false;
	}

	if ((numerator_ * other.denominator_) >= (other.numerator_ * denominator_))
	{
		return false;
	}

	return true;
}

bool Rational::operator<=(const Rational& other) const {
	return *this < other || *this == other;
}

Rational Rational::operator+() const {
	return *this;
}

//check
Rational Rational::operator-() const {
	Rational result(*this);
	if (this->numerator_ != 0)
	{
		result.numerator_ = -numerator_;
	}

	return result;
}

Rational& Rational::operator+=(const Rational& other) {
	if (other.denominator_ == denominator_)
	{
		numerator_ += other.numerator_;
		denominator_ = other.denominator_;

		BigInteger temp = -numerator_;
		if ((numerator_ == 0) || (temp == 0))
		{
			numerator_ = 0;
			denominator_ = 1;
		}
		else {
			(*this).reduction();
		}

		return *this;
	}

	numerator_ = numerator_ * other.denominator_ + denominator_ * other.numerator_;
	denominator_ *= other.denominator_;

	BigInteger temp = -numerator_;
	if ((numerator_ == 0) || (temp == 0))
	{
		numerator_ = 0;
		denominator_ = 1;
	}
	else {
		(*this).reduction();
	}

	return *this;
}

Rational& Rational::operator-=(const Rational& other) {
	*this += -other;

	return *this;
}

Rational& Rational::operator*=(const Rational& other) {
	numerator_ *= other.numerator_;
	denominator_ *= other.denominator_;

	BigInteger temp = -numerator_;
	if ((numerator_ == 0) || (temp == 0))
	{
		numerator_ = 0;
		denominator_ = 1;
	}
	else {
		(*this).reduction();
	}

	return *this;
}

Rational& Rational::operator/=(const Rational& other) {
	numerator_ *= other.denominator_;

	if (other.numerator_ < 0)
	{
		denominator_ *= -other.numerator_;
		if (other.numerator_ < 0)
		{
			numerator_ = -numerator_;
		}
	}
	else {
		denominator_ *= other.numerator_;
	}

	BigInteger temp = -numerator_;
	if ((numerator_ == 0) || (temp == 0))
	{
		numerator_ = 0;
		denominator_ = 1;
	}
	else {
		(*this).reduction();
	}

	return *this;
}

Rational operator+(Rational left, Rational const & right) {
	return left += right;
}

Rational operator-(Rational left, Rational const & right) {
	return left -= right;
}

Rational operator*(Rational left, Rational const & right) {
	return left *= right;
}

Rational operator/(Rational left, Rational const & right) {
	return left /= right;
}

std::ostream& operator<<(std::ostream& output, Rational& other) {
	if (other.denominator() == 1)
	{
		output << to_string(other.numerator());
	}
	else {
		output << to_string(other.numerator()) << '/' << to_string(other.denominator());
	}

	return output;
}

std::string to_string(Rational& other) {
	std::string result;
	if (other.denominator() == 1)
	{
		result = to_string(other.numerator());
	}
	else {
		result += to_string(other.numerator()) + "/" + to_string(other.denominator());
	}

	return result;
}

std::ostream& disp(std::ostream& output, Rational& other) {
	if (other.denominator() == 1)
	{
		output << to_string(other.numerator());
	}
	else {
		output << to_string(other.numerator()) << '/' << to_string(other.denominator());
	}

	return output;
}
