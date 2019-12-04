#include "NeuralNetwork.h"
#include <iostream>

#define SIGMOID (func)([](float x) {return 1.0f / (1.0f + exp(-x));})
#define D_SIGMOID (func)([](float y) {return y*(1.0f-y);})

NeuralNetwork::NeuralNetwork(int* nodes, int length, float lr)
	: Nodes(nodes)
	, Length(length)
	, learning_rate(lr)
{
	weights = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		weights[i] = new Matrix(nodes[i + 1], nodes[i]);
		weights[i]->Randomize(-2.0f, 2.0f);
	}
	biases = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		biases[i] = new Matrix(nodes[i + 1], 1);
		biases[i]->Randomize(-2.0f,2.0f);
	}
}

NeuralNetwork::~NeuralNetwork()
{
	delete[] weights;
	delete[] biases;
}

Matrix* NeuralNetwork::FeedForward(Matrix* inputs) const
{
	Matrix** layerOutputs = new Matrix * [Length];
	layerOutputs[0] = inputs;
	for (int i = 1; i < Length; i++)
	{
		layerOutputs[i] = Matrix::MatrixMultiplication(weights[i - 1], layerOutputs[i - 1]);
		*layerOutputs[i] += *biases[i - 1];
		layerOutputs[i]->Activation(SIGMOID);
	}
	Matrix* result = new Matrix(*layerOutputs[Length - 1]);
	for (int i = 0; i < Length; i++)
	{
		delete layerOutputs[i];
	}
	delete[] layerOutputs;
	return result;
}

void NeuralNetwork::FeedForward(Matrix** outputs) const
{
	for (int i = 1; i < Length; i++)
	{
		outputs[i] = Matrix::MatrixMultiplication(weights[i - 1], outputs[i - 1]);
		*outputs[i] += *biases[i - 1];
		outputs[i]->Activation(SIGMOID);
	}
}

void NeuralNetwork::Train(Matrix* inputs, Matrix* targets)
{
	Matrix** outputs = new Matrix * [Length];
	outputs[0] = inputs;
	FeedForward(outputs);

	Matrix** errors = new Matrix * [Length];
	errors[Length - 1] = *targets - *outputs[Length - 1];
	delete targets;

	for (int i = Length - 2; i >=0; i--)
	{
		Matrix* T = Matrix::Transpose(weights[i]);
		errors[i] = Matrix::MatrixMultiplication(T, errors[i + 1]);
		delete T;
	}

	for (int i = Length - 2; i >= 0; i--)
	{
		Matrix* gradients = Matrix::Map(outputs[i+1],D_SIGMOID);

		*gradients *= *errors[i+1];
		*gradients *= learning_rate;
		Matrix* T = Matrix::Transpose(outputs[i]);
		Matrix* deltas = Matrix::MatrixMultiplication(gradients, T);

		*weights[i] += *deltas;
		*biases[i] += *gradients;

		delete gradients;
		delete T;
		delete deltas;
	}
	for (int i = 0; i < Length; i++)
	{
		delete outputs[i];
		delete errors[i];
	}
	delete[] outputs;
	delete[] errors;
}
