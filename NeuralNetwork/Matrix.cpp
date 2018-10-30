#include "Matrix.h"

Matrix::Matrix()
	:rows(-1)
	, columns(-1)
{

}
Matrix::Matrix(int r, int c)
	:rows(r)
	, columns(c)
{
	this->data = new double*[rows];
	for (int i = 0; i < rows; ++i)
		this->data[i] = new double[columns];
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			this->data[i][j] = 0.0;
}
Matrix::Matrix(Matrix& copy)
	: rows(copy.getRows())
	, columns(copy.getColumns())
{

}
Matrix::~Matrix()
{
}
void Matrix::sum(Matrix* m)
{
	if (this->getColumns() != m->getColumns() || this->getRows != m->getRows())
		return;
	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < this->getColumns(); j++)
		{
			this->getData()[i][j] += m->getData()[i][j];
		}
	}
}
void Matrix::elementarySum(double num)
{
	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < this->getColumns(); j++)
		{
			this->getData()[i][j] += num;
		}
	}
	return;
}
void Matrix::elementaryMult(double num)
{
	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < this->getColumns(); j++)
		{
			this->getData()[i][j] *= num;
		}
	}
	return;
}
Matrix* Matrix::matrixMult()const
{
	return  new Matrix();
}
Matrix* Matrix::transpose()const
{
	return  new Matrix();
}
