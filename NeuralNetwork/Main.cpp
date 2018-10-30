#include "Matrix.h"
#include <iostream>
using namespace::std;
int main(int argc, char *argv[]) 
{
	Matrix* m = new Matrix(5, 6);
	m->elementarySum(5);
	m->getData()[3][0] = 10.0;
	cout << m->getData()[2][0];
	cout << m->getData()[3][0];
	return 0;
}