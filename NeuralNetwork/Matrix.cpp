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
Matrix* Matrix::sum()const
{
	return  new Matrix();
}
Matrix* Matrix::elementaryMult()const
{
	return  new Matrix();
}
Matrix* Matrix::matrixMult()const
{
	return  new Matrix();
}
Matrix* Matrix::transpose()const
{
	return  new Matrix();
}
