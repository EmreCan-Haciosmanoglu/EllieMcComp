#include "Matrix.h"

Matrix::Matrix()
	:rows(-1)
	, columns(-1)
{
	data = new float[0];
}
Matrix::Matrix(int r, int c)
	: rows(r)
	, columns(c)
{
	data = new float[rows * columns];
	for (int i = 0; i < rows; ++i)
		data[i] = 0.0f;
}

Matrix::~Matrix()
{
}
Matrix* Matrix::MatrixMultiplication(Matrix& other)
{
	int otr = other.getRows();
	if (getColumns() != otr)
		return;
	Matrix *tmp = new Matrix(getRows(), other.getColumns());
	float* d = tmp->getData();
	int tc = tmp->getColumns();
	int tr = tmp->getRows();
	for (int j = 0; j < tr; j++)
		for (int i = 0; i < tc; i++)
		{
			d[j * tc + i] = 0;
			for (int k = 0; k < otr; k++)
				d[j * tc + i] += this->getData()[j * otr + k] * other.getData()[k * tc + i];
		}
	return tmp;
}

void Matrix::transpose()
{
	Matrix tmp(getColumns(), getRows());

	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getColumns(); j++)
		{
			tmp.getData()[j][i] = getData()[i][j];
		}
	}
	setColumns(tmp.getColumns());
	setRows(tmp.getRows());
	setData(tmp.getData());
}

Matrix& Matrix::operator+(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] += x;

	return *this;
}
Matrix& Matrix::operator+(Matrix& other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] += other.getData()[j * columns + i];

	return *this;
}

Matrix & Matrix::operator-(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] -= x;

	return *this;
}
Matrix& Matrix::operator-(Matrix& other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] -= other.getData()[j * columns + i];

	return *this;
}

Matrix & Matrix::operator*(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] *= x;

	return *this;
}
Matrix& Matrix::operator*(Matrix& other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] *= other.getData()[j * columns + i];

	return *this;
}

Matrix & Matrix::operator/(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] /= x;

	return *this;
}
Matrix& Matrix::operator/(Matrix& other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] /= other.getData()[j * columns + i];

	return *this;
}
