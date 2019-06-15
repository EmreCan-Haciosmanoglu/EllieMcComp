#pragma once
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int, int*, int, int);

	~NeuralNetwork();

	Matrix* FeedForward(Matrix& input) const;
	void Train( Matrix* inputs,  Matrix* targets);
private:
	int inputCount;
	int* hiddenCount;
	int hiddenLength;
	int outputCount;

	Matrix* weights_ih;
	Matrix** weights_hh;
	Matrix* weights_ho;

	Matrix** bias_h;
	Matrix* bias_o;

	float learning_rate;
};

