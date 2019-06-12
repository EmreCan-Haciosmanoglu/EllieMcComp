#include "Matrix.h"

#include <ostream>

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
	int w = r * c;
	data = new float[w];
	for (int i = 0; i < rows * columns; i++)
		data[i] = 0.0f;
}

Matrix::~Matrix()
{
	delete[] data;
}

Matrix* Matrix::MatrixMultiplication(Matrix & other)
{
	int otr = other.getRows();
	if (getColumns() != otr)
		return nullptr;
	Matrix * tmp = new Matrix(getRows(), other.getColumns());
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
Matrix& Matrix::Transpose()
{
	float* temp = new float[rows * columns];

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			temp[i * rows + j] = data[j * columns + i];

	int c = columns;
	setColumns(rows);
	setRows(c);
	setData(temp);

	return *this;
}

Matrix & Matrix::operator+(float x)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] += x;

	return *this;
}
Matrix & Matrix::operator+(Matrix & other)
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
Matrix & Matrix::operator-(Matrix & other)
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
Matrix & Matrix::operator*(Matrix & other)
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
Matrix & Matrix::operator/(Matrix & other)
{
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			data[j * columns + i] /= other.getData()[j * columns + i];

	return *this;
}



std::ostream & operator<<(std::ostream & stream, Matrix & matrix)
{
	int rows = matrix.getRows();
	int cols = matrix.getColumns();
	float* data = matrix.getData();

	stream << "Matrix: " << rows << " - " << cols << "\n[ ";

	for (int j = 0; j < rows - 1; j++)
	{
		stream << data[j * cols];
		for (int i = 1; i < cols; i++)
		{
			stream << ", " << data[j * cols + i];
		}
		stream << std::endl;
	}
	stream << ", " << data[rows * cols - 1] << "]\n";


	return stream;
}