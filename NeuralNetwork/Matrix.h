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
	void transpose();

	Matrix& operator+(float x);
	Matrix& operator+(Matrix& other);
	Matrix& operator+=(float x) { return *this + x; }
	Matrix& operator+=(Matrix& other) { return *this + other; }

	Matrix& operator-(float x);
	Matrix& operator-(Matrix& other);
	Matrix& operator-=(float x) { return *this - x; }
	Matrix& operator-=(Matrix& other) { return *this - other; }

	Matrix& operator*(float x);
	Matrix& operator*(Matrix& other);
	Matrix& operator*=(float x) { return *this * x; }
	Matrix& operator*=(Matrix& other) { return *this * other; }

	Matrix& operator/(float x);
	Matrix& operator/(Matrix& other);
	Matrix& operator/=(float x) { return *this / x; }
	Matrix& operator/=(Matrix& other) { return *this / other; }



public:
	inline int getRows() const { return rows; }
	inline int getColumns() const { return columns; }
	inline float* getData() { return data; }

	void setRows(int r) { rows = r; }
	void setColumns(int c) { columns = c; }
	void setData(float* d) { data = d; }

private:
	int rows;
	int columns;
	float* data;
};

