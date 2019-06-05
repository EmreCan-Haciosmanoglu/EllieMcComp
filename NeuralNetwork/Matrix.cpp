#include "Matrix.h"

Matrix::Matrix()
	:rows(-1)
	, columns(-1)
{
	data = new double* [0];
}
Matrix::Matrix(int r, int c)
	: rows(r)
	, columns(c)
{
	data = new double* [rows];
	for (int i = 0; i < rows; ++i)
		data[i] = new double[columns];
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] = 0.0;
}
Matrix::Matrix(const Matrix & copy)
{
	columns = copy.columns;
	rows = copy.rows;
	data = new double* [rows];
	for (int i = 0; i < rows; ++i)
		data[i] = new double[columns];
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] = copy.data[i][j];
}
Matrix::~Matrix()
{
}

void Matrix::sum(Matrix m)
{
	if ((getColumns() != m.getColumns()) || (getRows() != m.getRows()))
		return;
	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getColumns(); j++)
		{
			getData()[i][j] += m.getData()[i][j];
		}
	}
}

void Matrix::elementarySum(double num)
{
	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getColumns(); j++)
		{
			getData()[i][j] += num;
		}
	}
	return;
}
void Matrix::elementaryMult(double num)
{
	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getColumns(); j++)
		{
			getData()[i][j] *= num;
		}
	}
	return;
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