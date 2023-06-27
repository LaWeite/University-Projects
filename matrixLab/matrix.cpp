#include "matrix.h"
#include <iomanip>

Matrix::Matrix() {
	n_ = 1;
	m_ = 0;
	matrix_ = new Rational * [n_];
	for (int i = 0; i < n_; ++i)
	{
		matrix_[i] = new Rational[m_];
	}
}

Matrix::Matrix(const Matrix& other) : n_(other.n_), m_(other.m_) {
	matrix_ = new Rational * [other.n_];
	for (int i = 0; i < other.n_; ++i)
	{
		matrix_[i] = new Rational[other.m_];
	}

	for (int rows = 0; rows < n_; ++rows)
	{
		for (int columns = 0; columns < m_; ++columns)
		{
			matrix_[rows][columns] = other.matrix_[rows][columns];
		}
	}
}

Matrix::Matrix(Rational number) {
	n_ = 1;
	m_ = 1;

	matrix_ = new Rational * [n_];
	for (int i = 0; i < n_; ++i)
	{
		matrix_[i] = new Rational[m_];
	}

	matrix_[0][0] = number;
}

Matrix& Matrix::operator=(const Matrix& other) {
	n_ = other.n_;
	m_ = other.m_;

	matrix_ = new Rational * [other.n_];
	for (int i = 0; i < other.n_; ++i)
	{
		matrix_[i] = new Rational[other.m_];
	}

	for (int rows = 0; rows < n_; ++rows)
	{
		for (int columns = 0; columns < m_; ++columns)
		{
			matrix_[rows][columns] = other.matrix_[rows][columns];
		}
	}

	return *this;
}

bool operator==(Matrix left, Matrix right) {
	if ((left.n_ != right.n_) || (left.m_ != right.n_))
	{
		return false;
	}

	for (int rows = 0; rows < left.n_; ++rows)
	{
		for (int columns = 0; columns < left.m_; ++columns)
		{
			if (left.matrix_[rows][columns] != right.matrix_[rows][columns])
			{
				return false;
			}
		}
	}

	return true;
}

Matrix Matrix::operator-() const {
	Matrix result(*this);
	for (int rows = 0; rows < result.n_; ++rows)
	{
		for (int columns = 0; columns < result.m_; ++columns)
		{
			result.matrix_[rows][columns] = -result.matrix_[rows][columns];
		}
	}

	return result;
}

Matrix Matrix::operator+() const {
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
	for (int rows = 0; rows < other.n_; ++rows)
	{
		for (int columns = 0; columns < other.m_; ++columns)
		{
			matrix_[rows][columns] += other.matrix_[rows][columns];
		}
	}

	return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
	for (int rows = 0; rows < n_; ++rows)
	{
		for (int columns = 0; columns < m_; ++columns)
		{
			matrix_[rows][columns] -= other.matrix_[rows][columns];
		}
	}

	return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
	if (other.n_ == 1 && other.m_ == 1)
	{
		for (int rows = 0; rows < n_; ++rows)
		{
			for (int columns = 0; columns < m_; ++columns)
			{
				matrix_[rows][columns] *= other.matrix_[0][0];
			}
		}

		return *this;
	}

	for (int rows = 0; rows < n_; ++rows)
	{
		for (int columns = 0; columns < m_; ++columns)
		{
			matrix_[rows][columns] *= other.matrix_[rows][columns];
		}
	}

	return *this;
}

Matrix& Matrix::operator/=(const Matrix& other) {
	for (int rows = 0; rows < other.n_; ++rows)
	{
		for (int columns = 0; columns < other.m_; ++columns)
		{
			matrix_[rows][columns] /= other.matrix_[rows][columns];
		}
	}

	return *this;
}

Matrix operator+(Matrix left, Matrix right) {
	return left += right;
}

Matrix operator-(Matrix left, Matrix right) {
	return left -= right;
}

Matrix operator*(Matrix left, Matrix right) {
	if (left.n_ == 1 && left.m_ == 1)
	{
		return right *= left;
	}

	return left *= right;
}

Matrix operator/(Matrix left, Matrix right) {
	return left /= right;
}

Matrix matrix_prod(Matrix left, Matrix right) {
	Matrix result;
	result.n_ = left.n_;
	result.m_ = right.m_;

	result.matrix_ = new Rational* [result.n_];
	for (int i = 0; i < result.n_; ++i)
	{
		result.matrix_[i] = new Rational[result.m_];
	}

	for (int i = 0; i < left.n_; ++i) {
		for (int j = 0; j < right.m_; ++j) {
			Rational temp(0);
			for (int k = 0; k < left.m_; ++k) {
				temp += left.matrix_[i][k] * right.matrix_[k][j];
			}
			result.matrix_[i][j] = temp;
		}
	}

	return result;
}

void zeros(Matrix& matrix, int n, int m) {
	matrix.n_ = n;
	matrix.m_ = m;

	matrix.matrix_ = new Rational* [n];
	for (int i = 0; i < n; ++i)
	{
		matrix.matrix_[i] = new Rational[m];
	}

	for (int rows = 0; rows < n; ++rows)
	{
		for (int columns = 0; columns < m; ++columns)
		{
			matrix.matrix_[rows][columns] = Rational(0);
		}
	}
}

void ones(Matrix& matrix, int n, int m) {
	matrix.n_ = n;
	matrix.m_ = m;

	matrix.matrix_ = new Rational * [n];
	for (int i = 0; i < n; ++i)
	{
		matrix.matrix_[i] = new Rational[m];
	}

	for (int rows = 0; rows < n; ++rows)
	{
		for (int columns = 0; columns < m; ++columns)
		{
			matrix.matrix_[rows][columns] = Rational(1);
		}
	}
}

void eye(Matrix& matrix, int n) {
	matrix.n_ = n;
	matrix.m_ = n;

	matrix.matrix_ = new Rational * [n];
	for (int i = 0; i < n; ++i)
	{
		matrix.matrix_[i] = new Rational[n];
	}

	for (int rows = 0; rows < n; rows++)
	{
		for (int columns = 0; columns < n; columns++)
		{
			matrix.matrix_[rows][columns] = Rational(1);
		}
	}
}

void linspace(Matrix& matrix, int x0, int x1, int n) {
	matrix.n_ = 1;
	matrix.m_ = n;

	matrix.matrix_ = new Rational* [1];

	matrix.matrix_[0] = new Rational[n];

	int temp = 0;
	for (int i = x0; i <= x1; ++i)
	{
		matrix.matrix_[0][temp++] = i;
	}
}

Matrix vertcat(Matrix& left, Matrix& right) {
	Matrix result;
	result.n_ = left.n_ + right.n_;
	result.m_ = left.m_;

	result.matrix_ = new Rational * [result.n_];
	for (int i = 0; i < result.n_; ++i)
	{
		result.matrix_[i] = new Rational[result.m_];
	}

	for (int rows = 0; rows < left.n_; ++rows) {
		for (int columns = 0; columns < left.m_; ++columns) {

			result.matrix_[rows][columns] = left.matrix_[rows][columns];
		}
	}

	for (int rows = 0; rows < right.n_; ++rows) {
		for (int columns = 0; columns < right.m_; ++columns) {

			result.matrix_[left.n_ + rows][columns] = right.matrix_[rows][columns];
		}
	}

	return result;
}

Matrix horzcat(Matrix& left, Matrix& right) {
	Matrix result;
	result.n_ = left.n_;
	result.m_ = left.m_ + right.m_;

	result.matrix_ = new Rational* [result.n_];
	for (int i = 0; i < result.n_; ++i)
	{
		result.matrix_[i] = new Rational[result.m_];
	}

	for (int rows = 0; rows < left.n_; ++rows) {
		for (int columns = 0; columns < left.m_; ++columns) {

			result.matrix_[rows][columns] = left.matrix_[rows][columns];
		}
	}

	for (int rows = 0; rows < right.n_; ++rows) {
		for (int columns = 0; columns < right.m_; ++columns) {

			result.matrix_[rows][left.m_ + columns] = right.matrix_[rows][columns];
		}
	}

	return result;
}

Matrix transpose(Matrix& matrix) {
	Matrix matrix_new;
	matrix_new.m_ = matrix.n_;
	matrix_new.n_ = matrix.m_;

	matrix_new.matrix_ = new Rational * [matrix_new.n_];
	for (int i = 0; i < matrix_new.n_; ++i)
	{
		matrix_new.matrix_[i] = new Rational[matrix_new.m_];
	}

	for (int rows = 0; rows < matrix_new.n_; ++rows)
	{
		for (int columns = 0; columns < matrix_new.m_; ++columns)
		{
			matrix_new.matrix_[rows][columns] = matrix.matrix_[columns][rows];
		}
	}

	return matrix_new;
}

int LU_decompose(Matrix& matrix, int size, Rational* p) {
	int index_max;
	Rational j;
	Rational max_matrix;
	Rational* ptr;
	Rational absolute_matrix;

	for (int i = 0; i <= size; i++) {
		p[i] = i;
	}

	for (int i = 0; i < size; i++) {
		max_matrix = 0;
		index_max = i;

		for (int k = i; k < size; k++) {
			if (matrix.matrix_[k][i] < 0)
			{
				absolute_matrix = -matrix.matrix_[k][i];
			}
			else {
				absolute_matrix = matrix.matrix_[k][i];
			}

			if (absolute_matrix > max_matrix) {
				max_matrix = absolute_matrix;
				index_max = k;
			}
		}

		if (index_max != i) {
			j = p[i];
			p[i] = p[index_max];
			p[index_max] = j;

			ptr = matrix.matrix_[i];
			matrix.matrix_[i] = matrix.matrix_[index_max];
			matrix.matrix_[index_max] = ptr;

			p[size] += 1;
		}

		for (int j = i + 1; j < size; j++) {
			matrix.matrix_[j][i] /= matrix.matrix_[i][i];
			for (int k = i + 1; k < size; k++) {
				matrix.matrix_[j][k] -= matrix.matrix_[j][i] * matrix.matrix_[i][k];
			}
		}
	}

	return 1;
}

Rational determinant_of_matrix_with_LU(Matrix& matrix, Rational* p, int size) {

	Rational determinant = matrix.matrix_[0][0];

	for (int i = 1; i < size; i++) {
		determinant *= matrix.matrix_[i][i];
	}

	if ((p[size] - size).numerator() % 2 == 0)
	{
		return determinant;
	}
	else {
		return -determinant;
	}
}

Rational det(Matrix& matrix) {
	Rational result;

	Rational* p = new Rational[matrix.n_ + 1];

	LU_decompose(matrix, matrix.n_, p);

	result = determinant_of_matrix_with_LU(matrix, p, matrix.n_);

	return result;
}

void invert_matrix_with_LU(Matrix& matrix, const Rational* p, int size, Matrix& result_invert_matrix) {
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			result_invert_matrix.matrix_[i][j] = p[i] == j ? 1 : 0;

			for (int k = 0; k < i; k++) {
				result_invert_matrix.matrix_[i][j] -= matrix.matrix_[i][k] * result_invert_matrix.matrix_[k][j];
			}
		}

		for (int i = size - 1; i >= 0; i--) {
			for (int k = i + 1; k < size; k++) {
				result_invert_matrix.matrix_[i][j] -= matrix.matrix_[i][k] * result_invert_matrix.matrix_[k][j];
			}

			result_invert_matrix.matrix_[i][j] /= matrix.matrix_[i][i];
		}
	}
}

Matrix inv(Matrix& matrix) {
	Matrix result;

	result.n_ = matrix.n_;
	result.m_ = matrix.m_;

	result.matrix_ = new Rational * [result.n_];
	for (int i = 0; i < result.n_; i++)
	{
		result.matrix_[i] = new Rational[result.m_];
	}

	Rational check;

	Rational* p = new Rational[matrix.n_ + 1];

	LU_decompose(matrix, matrix.n_, p);

	check = determinant_of_matrix_with_LU(matrix, p, matrix.n_);

	invert_matrix_with_LU(matrix, p, matrix.n_, result);

	return result;
}

Matrix matrix_del(Matrix left, Matrix right) {
	Matrix result;
	result.n_ = right.n_;
	result.m_ = right.m_;

	result.matrix_ = new Rational * [result.n_];
	for (int i = 0; i < result.n_; ++i)
	{
		result.matrix_[i] = new Rational[result.m_];
	}

	Matrix invert = inv(right);
	result = matrix_prod(left, invert);

	return result;
}


Rational sum(Matrix& matrix) {
	Rational result = 0;
	for (int rows = 0; rows < matrix.n_; ++rows)
	{
		for (int columns = 0; columns < matrix.m_; ++columns)
		{
			result += matrix.matrix_[rows][columns];
		}
	}

	result.reduction();

	return result;
}

Rational prod(Matrix& matrix) {
	Rational result = 1;
	for (int rows = 0; rows < matrix.n_; ++rows)
	{
		for (int columns = 0; columns < matrix.m_; ++columns)
		{
			result *= matrix.matrix_[rows][columns];
		}
	}

	result.reduction();

	return result;
}

Matrix min(Matrix& left, Matrix& right) {
	Matrix matrix_result;
	matrix_result.n_ = left.n_;
	matrix_result.m_ = left.m_;

	matrix_result.matrix_ = new Rational * [matrix_result.n_];
	for (int i = 0; i < matrix_result.n_; i++)
	{
		matrix_result.matrix_[i] = new Rational[matrix_result.m_];
	}

	for (int rows = 0; rows < left.n_; rows++)
	{
		for (int columns = 0; columns < left.n_; columns++)
		{
			if (left.matrix_[rows][columns] <= right.matrix_[rows][columns])
			{
				matrix_result.matrix_[rows][columns] = left.matrix_[rows][columns];
			}
			else {
				matrix_result.matrix_[rows][columns] = right.matrix_[rows][columns];
			}
		}
	}

	return matrix_result;
}

Matrix max(Matrix& left, Matrix& right) {
	Matrix matrix_result;
	matrix_result.n_ = left.n_;
	matrix_result.m_ = left.m_;

	matrix_result.matrix_ = new Rational * [matrix_result.n_];
	for (int i = 0; i < matrix_result.n_; i++)
	{
		matrix_result.matrix_[i] = new Rational[matrix_result.m_];
	}

	for (int rows = 0; rows < left.n_; rows++)
	{
		for (int columns = 0; columns < left.n_; columns++)
		{
			if (left.matrix_[rows][columns] >= right.matrix_[rows][columns])
			{
				matrix_result.matrix_[rows][columns] = left.matrix_[rows][columns];
			}
			else {
				matrix_result.matrix_[rows][columns] = right.matrix_[rows][columns];
			}
		}
	}

	return matrix_result;
}

std::ostream& operator<<(std::ostream& output, Matrix& other) {
	long long length = 0;
	long long templength = 0;
	for (int rows = 0; rows < other.n_; ++rows)
	{
		for (int columns = 0; columns < other.m_; ++columns)
		{
			for (int i = 0; i < other.n_; ++i)
			{
				std::string numeratorStr = to_string(other.matrix_[i][columns].numerator());
				std::string denominatorStr = to_string(other.matrix_[i][columns].denominator());
				templength = numeratorStr.length() + denominatorStr.length();
				if (other.matrix_[i][columns].denominator() != 1)
				{
					++templength;
				}
				else {
					--templength;
				}

				if (templength > length)
				{
					length = templength;
				}
			}

			if (other.n_ == 1 && other.m_ == 1)
			{
				output << other.matrix_[rows][columns];
			}
			else {
				output << std::setw(length) << to_string(other.matrix_[rows][columns]);
			}
			if (columns != other.m_ - 1)
			{
				output << ' ';
			}
			
			length = 0;
		}
		if (rows != other.n_ - 1)
		{
			output << '\n';
		}
	}

	return output;
}

std::ostream& disp(std::ostream& output, Matrix& other) {
	output << other;

	return output;
}
