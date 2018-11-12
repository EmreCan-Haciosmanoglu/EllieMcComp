#pragma once
class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int * neurons, int size);
	NeuralNetwork(const NeuralNetwork& copy);
	~NeuralNetwork();

	int* guess( int* input);
private:
	double***  weights;
};

