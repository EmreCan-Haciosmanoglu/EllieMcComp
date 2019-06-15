#include "NeuralNetwork.h"
#include <iostream>

#define SIGMOID (func)([](float x) {return 1 / (1 + exp(-x));})
#define D_SIGMOID (func)([](float y) {return y*(1-y);})

NeuralNetwork::NeuralNetwork()
	: inputCount(0)
	, hiddenLength(1)
	, outputCount(0)
	, weights_ih(nullptr)
	, weights_hh(nullptr)
	, weights_ho(nullptr)
	, bias_h(nullptr)
	, bias_o(nullptr)
	, learning_rate(0.0f)
{
	hiddenCount = new int[0];
}

NeuralNetwork::NeuralNetwork(int input, int* hidden, int hiddenL, int output)
	: inputCount(input)
	, hiddenCount(hidden)
	, hiddenLength(hiddenL)
	, outputCount(output)
	, learning_rate(0.01f)
{
	weights_ih = new Matrix(hidden[0], input);
	weights_hh = new Matrix * [hiddenL - 1];
	for (int i = 0; i < hiddenL - 1; i++)
		weights_hh[i] = new Matrix(hidden[i + 1], hidden[i]);
	weights_ho = new Matrix(output, hidden[hiddenL - 1]);

	bias_h = new Matrix * [hiddenL];
	for (int i = 0; i < hiddenL; i++)
		bias_h[i] = new Matrix(hidden[i], 1);
	bias_o = new Matrix(output, 1);

}

NeuralNetwork::~NeuralNetwork()
{
	delete weights_ih;
	delete[] weights_hh;
	delete weights_ho;
	delete[] bias_h;
	delete bias_o;
}

Matrix* NeuralNetwork::FeedForward(Matrix & input) const
{
	Matrix* result = weights_ih->MatrixMultiplication(input);
	*result += *bias_h[0];
	result->Activation(SIGMOID);
	for (int i = 0; i < hiddenLength - 1; i++)
	{
		result = weights_hh[i]->MatrixMultiplication(*result);
		*result += *bias_h[i + 1];
		result->Activation(SIGMOID);
	}
	result = weights_ho->MatrixMultiplication(*result);
	*result += *bias_o;
	return result->Activation(SIGMOID);
}

void NeuralNetwork::Train(Matrix * inputs, Matrix * targets)
{
	Matrix** hiddens = new Matrix * [hiddenLength];

	hiddens[0] = (*(weights_ih->MatrixMultiplication(*inputs)) + *bias_h[0]).Activation(SIGMOID);
	for (int i = 1; i < hiddenLength; i++)
		hiddens[i] = (*(weights_ih->MatrixMultiplication(*hiddens[i - 1])) + *bias_h[0]).Activation(SIGMOID);

	Matrix * outputs = (*(weights_ih->MatrixMultiplication(*hiddens[hiddenLength - 1])) + *bias_h[0]).Activation(SIGMOID);
	Matrix * gradients = outputs->Activation(D_SIGMOID);

	Matrix& outputs_error = (*targets - *outputs);

	*gradients *= outputs_error;
	*gradients *= learning_rate;

	Matrix& weights_ho_deltas = (*gradients * *hiddens[hiddenLength - 1]);

	*weights_ho += weights_ho_deltas;

	for (int i = hiddenLength - 1; i > 0; i--)
	{

	}
}
