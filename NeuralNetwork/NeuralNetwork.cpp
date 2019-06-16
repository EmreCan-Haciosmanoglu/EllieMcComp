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

Matrix* NeuralNetwork::FeedForward(Matrix & input) const
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

void NeuralNetwork::Train(Matrix * inputs, Matrix * targets)
{
	Matrix** hiddens = new Matrix * [hiddenLength];

	hiddens[0] = (*(weights_ih->MatrixMultiplication(*inputs)) + *bias_h[0]).Activation(SIGMOID);
	for (int i = 1; i < hiddenLength; i++)
		hiddens[i] = (*(weights_ih->MatrixMultiplication(*hiddens[i - 1])) + *bias_h[0]).Activation(SIGMOID);

	Matrix * outputs = (*(weights_ih->MatrixMultiplication(*hiddens[hiddenLength - 1])) + *bias_h[0]).Activation(SIGMOID);
	Matrix * gradients = outputs->Activation(D_SIGMOID);

	Matrix & outputs_error = (*targets - *outputs);

	*gradients *= outputs_error;
	*gradients *= learning_rate;

	Matrix & weights_ho_deltas = (*gradients * *hiddens[hiddenLength - 1]);

	*weights_ho += weights_ho_deltas;

	for (int i = hiddenLength - 1; i > 0; i--)
	{

	}
}
