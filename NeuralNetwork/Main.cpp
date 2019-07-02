#include <iostream>

#include <time.h>
#include "NeuralNetwork.h"

#if 0
int main()
{
	srand(time(NULL));
	float arr1[6] = { 1,5,2,4,8,3 };
	float arr2[6] = { 4,-1,12,-6,4,1 };

	Matrix* m1 = new Matrix(3, 2, arr1);
	Matrix* m2 = new Matrix(3, 2, arr2);
	Matrix* m3 = *m1 + *m2;
	Matrix* m4 = *m1 - *m2;
	Matrix* m5 = *m1 * *m2;
	Matrix* m6 = *m1 / *m2;


	std::cout << "M1" << std::endl;
	m1->Print();
	std::cout << std::endl;
	std::cout << "M2" << std::endl;
	m2->Print();
	std::cout << std::endl;
	std::cout << "M3 << M1 + M2" << std::endl;
	m3->Print();
	std::cout << std::endl;
	std::cout << "M4 << M1 - M2" << std::endl;
	m4->Print();
	std::cout << std::endl;
	std::cout << "M5 << M1 * M2" << std::endl;
	m5->Print();
	std::cout << std::endl;
	std::cout << "M6 << M1 / M2" << std::endl;
	m6->Print();
	std::cout << std::endl;

	std::cout << std::endl;
	*m6 += *m1;
	std::cout << "M1" << std::endl;
	m1->Print();
	std::cout << std::endl;
	std::cout << "M6 << M6 + M1" << std::endl;
	m6->Print();
	std::cout << std::endl;
	std::cout << std::endl;

	Matrix* m7 = Matrix::Transpose(m6);
	std::cout << "M6" << std::endl;
	m6->Print();
	std::cout << std::endl;
	std::cout << "M7 << M6T" << std::endl;
	m7->Print();
	std::cout << std::endl;
	std::cout << std::endl;

	Matrix* m8 = Matrix::MatrixMultiplication(*m1, *Matrix::Transpose(m2));

	std::cout << "M1" << std::endl;
	m1->Print();
	std::cout << std::endl;
	std::cout << "M2" << std::endl;
	m2->Print();
	std::cout << std::endl;
	std::cout << "M2T" << std::endl;
	Matrix::Transpose(m2)->Print();
	std::cout << std::endl;
	std::cout << "M8 << M1 . M2T" << std::endl;
	m8->Print();


	delete m1;

	int wait = 0;
	std::cin >> wait;
	return 0;
}
#endif
#if 1
int main() {
	srand(time(NULL));
	int layers[3] = { 2, 5 ,2 };
	NeuralNetwork* nn = new NeuralNetwork(layers, 3, 0.01f);
	int times = 2000000;
	for (int i = 0; i < times; i++)
	{
		if (0 == i % (times / 100))
		{
			int perc = (i / (times / 100));
			//system("CLS");
			for (int i = 0; i < perc; i++)
			{
				std::cout << "|";
			}
			for (int i = 0; i < 100 - perc; i++)
			{
				std::cout << " ";
			}
			std::cout << "| " << perc << "%" << std::endl;
		}
		int r = rand() % 4;
		if (r == 0)
		{
			float in[2] = { 1,0 };
			float ta[2] = { 1,0 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 1)
		{
			float in[2] = { 0, 1 };
			float ta[2] = { 1 ,0 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 2)
		{
			float in[2] = { 0,0 };
			float ta[2] = { 0,1 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 3)
		{
			float in[2] = { 1,1 };
			float ta[2] = { 0,1 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
	}

	float inputs[4][2] = { {1,0},{0,1},{1,1},{0,0} };
	Matrix* input0 = new Matrix(2, 1, inputs[0]);
	Matrix* result0 = nn->FeedForward(input0);
	result0->Print();
	delete result0;

	Matrix* input1 = new Matrix(2, 1, inputs[1]);
	Matrix* result1 = nn->FeedForward(input1);
	result1->Print();
	delete result1;

	Matrix* input2 = new Matrix(2, 1, inputs[2]);
	Matrix* result2 = nn->FeedForward(input2);
	result2->Print();
	delete result2;

	Matrix* input3 = new Matrix(2, 1, inputs[3]);
	Matrix* result3 = nn->FeedForward(input3);
	result3->Print();
	delete result3;

	delete nn;
	int wait = 0;
	std::cin >> wait;
	return 0;
}
#endif