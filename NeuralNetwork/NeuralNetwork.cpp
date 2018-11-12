#include "NeuralNetwork.h"



NeuralNetwork::NeuralNetwork()
{
}

NeuralNetwork::NeuralNetwork(int * neurons, int size)
{
	weights = new double**[size - 1];

	for (int i = 0; i < size; i++)
		weights[i] = new double*[neurons[i + 1]];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < neurons[i + 1]; j++)
			weights[i][j] = new double[neurons[i]];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < neurons[i + 1]; j++)
			for (int k = 0; k < neurons[i]; k++)
				weights[i][j][k] = 0.0;
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork & copy)
{
}


NeuralNetwork::~NeuralNetwork()
{
}

int * NeuralNetwork::guess(int * input)
{
	return nullptr;
}
