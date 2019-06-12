#pragma once

class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);

	~Matrix();

	inline Matrix& Multiply(float x) { return (*this * x); }
	inline Matrix& Add(float x) { return *this + x; }

	Matrix* MatrixMultiplication(Matrix& other);
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
	inline Matrix& operator/=(float x) { return *this / x; }
	inline Matrix& operator/=(Matrix& other) { return *this / other; }



public:
	inline int getRows() const { return rows; }
	inline int getColumns() const { return columns; }
	inline float* getData() { return data; }

	inline void setRows(int r) { rows = r; }
	inline void setColumns(int c) { columns = c; }
	inline void setData(float* d) { data = d; }

private:
	int rows;
	int columns;
	float* data;
};

