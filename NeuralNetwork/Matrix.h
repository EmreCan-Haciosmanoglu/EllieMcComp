#pragma once
#include <cmath>

typedef float (*func)(float x);
class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);
	Matrix(int r, int c, float *d);

	~Matrix();

	inline Matrix& Multiply(float x) { return (*this * x); }
	inline Matrix& Add(float x) { return *this + x; }
	Matrix& Randomize(float min, float max);
	static Matrix* MatrixMultiplication(const Matrix& left, const Matrix& right);
	static Matrix* MatrixMultiplication(Matrix* left, Matrix* right);
	Matrix& Transpose();

	Matrix& operator+(float x);
	Matrix& operator+(Matrix& other);
	inline Matrix& operator+=(float x) { return *this + x; }
	inline Matrix& operator+=(Matrix& other) { return *this + other; }

	Matrix& operator-(float x);
	Matrix& operator-(Matrix& other);
	inline Matrix& operator-=(float x) { return *this - x; }
	inline Matrix& operator-=(Matrix& other) { return *this - other; }

	Matrix& operator*(float x);
	Matrix& operator*(Matrix& other);
	inline Matrix& operator*=(float x) { return *this * x; }
	inline Matrix& operator*=(Matrix& other) { return *this * other; }

	Matrix& operator/(float x);
	Matrix& operator/(Matrix& other);
	Matrix* Activation(func f);
	inline Matrix& operator/=(float x) { return *this / x; }
	inline Matrix& operator/=(Matrix& other) { return *this / other; }


	void Print() const;

public:
	inline int GetRows() const { return rows; }
	inline int GetColumns() const { return columns; }

	inline void SetRows(int r) { rows = r; }
	inline void SetColumns(int c) { columns = c; }
	inline void SetData(float* d) { data = d; }

private:
	int rows;
	int columns;
public:
	float* data;
};
