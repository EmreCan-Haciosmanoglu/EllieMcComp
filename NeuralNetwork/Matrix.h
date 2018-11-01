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
	void matrixMult(Matrix* m);
	void transpose(Matrix* m);

	int getRows() { return rows; }
	int getColumns() { return columns; }
	double** getData() { return data; }

	void setRows(int r) { rows = r; }
	void setColumns(int c) { columns = c; }
	void setData(double** d) { data = d; }
private:
	int rows;
	int columns;
	double** data;
};

