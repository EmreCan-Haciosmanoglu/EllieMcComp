#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);
	Matrix(Matrix& copy);
	~Matrix();

	Matrix* sum() const;
	Matrix* elementaryMult()const;
	Matrix* matrixMult()const;
	Matrix* transpose()const;
	int getRows() { return rows; }
	int getColumns() { return columns; }
	double** getData() { return data; }
private:
	int rows;
	int columns;
	double** data;
};

