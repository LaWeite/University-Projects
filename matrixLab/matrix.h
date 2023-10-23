#include <iostream>
#include "Rationalnumbers.h"

class Matrix {
public:
    int n_;
    int m_;
    Rational** matrix_;

    void swap(Matrix& other) {
		std::swap(this->n_, other.n_);
		std::swap(this->m_, other.m_);
		std::swap(this->matrix_, other.matrix_);
	}

    Matrix();
    Matrix(const Matrix& other);
    Matrix(Rational number);
    Matrix& operator=(const Matrix& other);

    Matrix operator-() const;
    Matrix operator+() const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator/=(const Matrix& other);
    operator int () const {
        int numerator = std::stoi(to_string(matrix_[0][0].numerator()));

        return numerator;
    }
    
    ~Matrix() {
        for (size_t i = 0; i < n_; ++i)
        {
            delete[] matrix_[i];
        }

        delete[] matrix_;
    }
};

Matrix operator+(Matrix left, Matrix const & right);
Matrix operator-(Matrix left, Matrix const & right);
Matrix operator*(Matrix left, Matrix right);
Matrix operator/(Matrix left, Matrix const & right);

bool operator==(Matrix const &left, Matrix const & right);

Matrix matrix_prod(Matrix const & left, Matrix const & right);
Matrix matrix_del(Matrix const & left, Matrix right);
void zeros(Matrix& matr, int n, int m);
void ones(Matrix& matr, int n, int m);
void eye(Matrix& matrix, int n);
void linspace(Matrix& matrix, int x0, int x1, int n);
Matrix vertcat(Matrix& left, Matrix& right);
Matrix horzcat(Matrix& left, Matrix& right);
Matrix transpose(Matrix& matrix_need);
Rational det(Matrix& matrix_need);
Matrix inv(Matrix& matrix_need);
Rational sum(Matrix& matrix_need);
Rational prod(Matrix& matrix_need);
Matrix min(Matrix& left, Matrix& right);
Matrix max(Matrix& left, Matrix& right);
std::ostream& operator<<(std::ostream& out, Matrix& other);
std::ostream& disp(std::ostream& out, Matrix& other);
