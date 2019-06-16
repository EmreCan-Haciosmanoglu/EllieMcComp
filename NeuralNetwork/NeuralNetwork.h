#pragma once
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork(int* nodes, int length, float lr);

	~NeuralNetwork();

	Matrix* FeedForward(Matrix& input) const;
	void Train(Matrix* inputs, Matrix* targets);

private:
	int* Nodes;
	int Length;

	Matrix** weights;
	Matrix** biases;
	Matrix** layerOutputs;

	float learning_rate;
};

