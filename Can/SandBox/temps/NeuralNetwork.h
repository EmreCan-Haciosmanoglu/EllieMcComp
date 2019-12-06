#pragma once
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork() = default;
	NeuralNetwork(int* nodes, int length, float lr);
	NeuralNetwork(int* nodes, int length, float lr, Matrix** w, Matrix** b);

	~NeuralNetwork();

	Matrix* FeedForward(Matrix* input) const;

	void FeedForward(Matrix** outputs) const;
	void Train(Matrix *inputs, Matrix* targets);

	static NeuralNetwork** Generate(NeuralNetwork** parents, float* fitnessScores, int parentCount, int childrenCount);
	static NeuralNetwork** Mutate(NeuralNetwork** source, float mutationRate);

	void SetWeights(Matrix** weights);
	void SetBiases(Matrix** biases);

	inline int* GetNodes() const { return Nodes; }
	inline Matrix** GetWeights() const { return weights; }
	inline Matrix** GetBiases() const { return biases; }
	inline int GetLength() const { return Length; }
	inline float GetLR() const { return learning_rate; }
private:
	int* Nodes;
	int Length;

	Matrix** weights;
	Matrix** biases;

	float learning_rate;
};

