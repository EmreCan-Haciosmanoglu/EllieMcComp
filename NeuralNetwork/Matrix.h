#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);
	Matrix(Matrix& copy);
	~Matrix();

	void sum(Matrix* m);
	void elementarySum(double num);
	void elementaryMult(double num);
	Matrix* matrixMult() const;
	Matrix* transpose() const;
	int getRows() { return rows; }
	int getColumns() { return columns; }
	double** getData() { return data; }
private:
	int rows;
	int columns;
	double** data;
};

