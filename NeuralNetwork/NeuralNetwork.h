#pragma once
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int, int*, int, int);

	~NeuralNetwork();

private:
	float* inputLayer;
	float** hiddenLayer;
	float* outputLayer;
};

