#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);

	~Matrix();

	inline Matrix& Multiply(float x) { return (*this * x); }
	inline Matrix& Add(float x) { return *this + x; }

	void matrixMult(Matrix m);
	void transpose();

	Matrix& operator+(float x);
	Matrix& operator+(const Matrix& other);
	Matrix& operator-(float x);
	Matrix& operator*(float x);
	Matrix& operator/(float x);


	Matrix& operator+=(float x);
	Matrix& operator-=(float x);
	Matrix& operator*=(float x);
	Matrix& operator/=(float x);

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

