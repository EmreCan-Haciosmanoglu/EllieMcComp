#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
	: inputLayer(nullptr)
	, hiddenLayer(new float*[0])
	, outputLayer(nullptr)
{
}

NeuralNetwork::NeuralNetwork(int input, int* hidden, int width, int output)
	: inputLayer(new float[input])
	, hiddenLayer(new float* [width])
	, outputLayer(new float[output])
{
	for (int i = 0; i < width; i++)
		hiddenLayer[i] = new float[hidden[i]];
}

NeuralNetwork::~NeuralNetwork()
{
}