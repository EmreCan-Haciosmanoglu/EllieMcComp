#include "Matrix.h"

#include <ostream>
#include <iostream>
#include <time.h>
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
	srand(time(NULL));
	int w = r * c;
	data = new float[w];
	for (int i = 0; i < w; i++)
		data[i] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2 - 1;
}
Matrix::Matrix(int r, int c, float *d)
	: rows(r)
	, columns(c)
	, data(d)
{
}

Matrix::~Matrix()
{
	delete[] data;
}

Matrix* Matrix::MatrixMultiplication(Matrix & other)
{
	int otr = other.GetRows();
	if (GetColumns() != otr)
		return nullptr;
	Matrix * tmp = new Matrix(GetRows(), other.GetColumns());
	float* d = tmp->GetData();
	int tc = tmp->GetColumns();
	int tr = tmp->GetRows();
	for (int j = 0; j < tr; j++)
		for (int i = 0; i < tc; i++)
		{
			d[j * tc + i] = 0;
			for (int k = 0; k < otr; k++)
				d[j * tc + i] += this->GetData()[j * otr + k] * other.GetData()[k * tc + i];
		}
	return tmp;
}
Matrix& Matrix::Transpose()
{
	int w = rows * columns;
	float* temp = new float[w];

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			temp[i * rows + j] = data[j * columns + i];

	int c = columns;
	SetColumns(rows);
	SetRows(c);
	SetData(temp);

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
			data[j * columns + i] += other.GetData()[j * columns + i];

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
			data[j * columns + i] -= other.GetData()[j * columns + i];

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
			data[j * columns + i] *= other.GetData()[j * columns + i];

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
			data[j * columns + i] /= other.GetData()[j * columns + i];

	return *this;
}

Matrix* Matrix::Activation(func f)
{
	int length = this->GetColumns() * this->GetRows();

	for (int i = 0; i < length; i++)
		data[i] = f(data[i]);

	return this;
}

void Matrix::Print() const
{

	int rows = this->GetRows();
	int cols = this->GetColumns();

	std::cout << "Matrix: " << rows << " - " << cols << "\n";

	for (int j = 0; j < rows; j++)
	{
		std::cout << data[j * cols];
		for (int i = 1; i < cols; i++)
		{
			std::cout << ", " << data[j * cols + i];
		}
		std::cout << std::endl;
	}
}



std::ostream& operator<<(std::ostream& stream, Matrix& matrix)
{
	int rows = matrix.GetRows();
	int cols = matrix.GetColumns();
	float* data = matrix.GetData();

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