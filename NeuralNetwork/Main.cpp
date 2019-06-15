#include <iostream>

#include <time.h>
#include "NeuralNetwork.h"


int main()
{
	srand(time(NULL));
	int hidden[1] = { 2 };
	NeuralNetwork *nn = new NeuralNetwork(2, hidden, 1, 1);

	float input[2] = { 1,0 };
	Matrix* inputM = new Matrix(2, 1, input);

	Matrix* output = nn->FeedForward(*inputM);

	output->Print();
	int i = 0;
	std::cin >> i;
	delete nn;
	return 0;
}