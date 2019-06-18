#include <iostream>

#include <time.h>
#include "NeuralNetwork.h"


int main()
{
	srand(time(NULL));
	int layers[3] = { 2, 2 ,1 };
	float inputs[4][2] = { {1,0},{0,1},{1,1},{0,0} };
	float targets[4][1] = { {1},{1},{0},{0} };
	NeuralNetwork* nn = new NeuralNetwork(layers, 3, 0.001f);

	for (int i = 0; i < 50000; i++)
	{
		int r = rand() % 4;
		Matrix input(2, 1, inputs[r]);
		Matrix target(1, 1, targets[r]);

		nn->Train(input, target);

		//delete input;
		//delete target;
	}

	Matrix* input0 = new Matrix(2, 1, inputs[0]);
	Matrix* input1 = new Matrix(2, 1, inputs[1]);
	Matrix* input2 = new Matrix(2, 1, inputs[2]);
	Matrix* input3 = new Matrix(2, 1, inputs[3]);
	nn->FeedForward(*input0)->Print();
	nn->FeedForward(*input1)->Print();
	nn->FeedForward(*input2)->Print();
	nn->FeedForward(*input3)->Print();

	int wait = 0;
	std::cin >> wait;
	delete nn;
	return 0;
}