#pragma once
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork(int* nodes, int length, float lr);

	~NeuralNetwork();

	Matrix* FeedForward(Matrix* input) const;
	void FeedForward(Matrix** outputs) const;
	void Train(Matrix *inputs, Matrix* targets);

private:
	int* Nodes;
	int Length;

	Matrix** weights;
	Matrix** biases;

	float learning_rate;
};

