#include <iostream>

#include "NeuralNetwork.h"


int main()
{
	int hidd[3] = { 3,8,6 };
	NeuralNetwork* brain = new NeuralNetwork(6, hidd, 3, 3);

	Matrix matrix1(2, 3);
	Matrix matrix2(3, 2);
	float d[6] = { 1,1,1,1,1,1 };
	float e[6] = { 1,1,1,1,1,1 };
	matrix1.setData(d);
	matrix2.setData(e);

	Matrix *matrix = matrix2.MatrixMultiplication(matrix1);

	int rows = matrix->getRows();
	int cols = matrix->getColumns();
	float* data = matrix->getData();

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

	int i = 1; 

	std::cin >> i;

	delete brain;
	return 0;
}