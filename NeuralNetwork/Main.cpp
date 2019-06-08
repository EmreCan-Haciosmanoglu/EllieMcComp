#include <iostream>

#include "NeuralNetwork.h"


int main()
{
	int hidd[3] = { 3,8,6 };
	NeuralNetwork* brain = new NeuralNetwork(6, hidd, 3, 3);

	delete brain;
	return 0;
}