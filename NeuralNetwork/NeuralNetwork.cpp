#include "NeuralNetwork.h"
#include <iostream>

#define SIGMOID (func)([](float x) {return 1 / (1 + exp(-x));})
#define D_SIGMOID (func)([](float y) {return y*(1-y);})

NeuralNetwork::NeuralNetwork(int* nodes, int length, float lr)
	: Nodes(nodes)
	, Length(length)
	, learning_rate(lr)
{
	weights = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		weights[i] = new Matrix(nodes[i + 1], nodes[i]);
		weights[i]->Randomize(-1.0f, 1.0f);
	}
	biases = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		biases[i] = new Matrix(nodes[i + 1], 1);
		biases[i]->Randomize(-1.0f, 1.0f);
	}
	layerOutputs = new Matrix * [length];
}

NeuralNetwork::~NeuralNetwork()
{
	delete[] weights;
	delete[] biases;
}

Matrix* NeuralNetwork::FeedForward(Matrix input) const
{
	layerOutputs[0] = &input;
	for (int i = 1; i < Length; i++)
	{
		layerOutputs[i] = Matrix::MatrixMultiplication(weights[i-1], layerOutputs[i-1]);
		*layerOutputs[i] += *biases[i-1];
		layerOutputs[i]->Activation(SIGMOID);
	}
	return layerOutputs[Length-1];
}

void NeuralNetwork::Train(Matrix inputs, Matrix targets)
{
	FeedForward(inputs);
	Matrix** errors = new Matrix * [Length];
	errors[Length - 1] = (targets - *layerOutputs[Length - 1]);

	for (int i = Length - 2; i >=0; i--)
	{
		Matrix* T = Matrix::Transpose(*weights[i]);
		errors[i] = Matrix::MatrixMultiplication(T, errors[i + 1]);
	}

	for (int i = Length - 2; i >= 0; i--)
	{
		Matrix* gradients = Matrix::Map(*layerOutputs[i+1],D_SIGMOID);

		*gradients *= *errors[i];
		*gradients *= learning_rate;
		Matrix* T = Matrix::Transpose(*layerOutputs[i]);
		Matrix* deltas = Matrix::MatrixMultiplication(gradients, T);

		*weights[i] += *deltas;
		*biases[i] += *gradients;
	}
}
