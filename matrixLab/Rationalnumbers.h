#include "Biginteger.h"

class Rational
{
private:
	BigInteger numerator_;
	BigInteger denominator_;
public:
	BigInteger numerator() { return numerator_; }
	BigInteger denominator() { return denominator_; }

	Rational();
	Rational(int i);
	Rational(BigInteger numerator, BigInteger denominator);

	Rational(const Rational& other);
	Rational(Rational&& other) noexcept;
	Rational& operator=(const Rational& other);
	Rational& operator=(Rational&& other) noexcept;

	bool operator!=(const Rational& other) const;
	bool operator>(const Rational& other) const;
	bool operator>=(const Rational& other) const;
	bool operator<(const Rational& other) const;
	bool operator<=(const Rational& other) const;

	Rational operator+() const;
	Rational operator-() const;

	Rational& operator+=(const Rational& other);
	Rational& operator-=(const Rational& other);
	Rational& operator*=(const Rational& other);
	Rational& operator/=(const Rational& other);

	void reduction();
};

bool operator==(Rational left, Rational right);

Rational operator+(Rational left, Rational right);
Rational operator-(Rational left, Rational right);
Rational operator*(Rational left, Rational right);
Rational operator/(Rational left, Rational right);

std::ostream& operator<<(std::ostream& out, Rational& other);
std::string to_string(Rational& other);
std::ostream& disp(std::ostream& out, Rational& other);
