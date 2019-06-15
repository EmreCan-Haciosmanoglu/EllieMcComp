#include "NeuralNetwork.h"
#include <iostream>


NeuralNetwork::NeuralNetwork()
	: inputCount(0)
	, hiddenLength(1)
	, outputCount(0)
	, weights_ih(nullptr)
	, weights_hh(nullptr)
	, weights_ho(nullptr)
	, bias_h(nullptr)
	, bias_o(nullptr)
{
	hiddenCount = new int[0];
}

NeuralNetwork::NeuralNetwork(int input, int* hidden, int hiddenL, int output)
	: inputCount(input)
	, hiddenCount(hidden)
	, hiddenLength(hiddenL)
	, outputCount(output)
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

Matrix* NeuralNetwork::FeedForward(Matrix& input) const
{
	Matrix* result = weights_ih->MatrixMultiplication(input);
	*result += *bias_h[0];
	result->Activation((func)([](float x) {return (1 / (1 + exp(-x))); }));
	for (int i = 0; i < hiddenLength - 1; i++)
	{
		result = weights_hh[i]->MatrixMultiplication(*result);
		*result += *bias_h[i + 1];
		result->Activation((func)([](float x) {return (1 / (1 + exp(-x))); }));
	}
	result = weights_ho->MatrixMultiplication(*result);
	*result += *bias_o;
	return result->Activation((func)([](float x) {return (1 / (1 + exp(-x))); }));
}