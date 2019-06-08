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
void Matrix::matrixMult(Matrix m)
{
	if (getColumns() != m.getRows())
		return;
	Matrix tmp(getRows(), m.getColumns());

	for (int i = 0; i < tmp.getRows(); i++)
	{
		for (int j = 0; j < tmp.getColumns(); j++)
		{
			tmp.getData()[i][j] = 0;
			for (int k = 0; k < m.getRows(); k++)
			{
				tmp.getData()[i][j] += getData()[i][k] * m.getData()[k][j];
			}

		}
	}
	setColumns(tmp.getColumns());
	setRows(tmp.getRows());
	setData(tmp.getData());
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

Matrix& Matrix::operator+(const Matrix& other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] += other[j * columns + i];

	return *this;
}

Matrix & Matrix::operator-(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] -= x;

	return *this;
}

Matrix & Matrix::operator*(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] *= x;

	return *this;
}

Matrix & Matrix::operator/(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] /= x;

	return *this;
}

Matrix& Matrix::operator+=(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] += x;

	return *this;
}

Matrix & Matrix::operator-=(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] -= x;

	return *this;
}

Matrix & Matrix::operator*=(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] *= x;

	return *this;
}

Matrix & Matrix::operator/=(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] /= x;

	return *this;
}
