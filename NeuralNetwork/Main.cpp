#include "Matrix.h"
#include <iostream>
using namespace::std;
int main(int argc, char *argv[]) 
{
	Matrix* m1 = new Matrix(5, 6);
	Matrix* m2 = new Matrix(5, 6);
	m1->elementarySum(5);
	m2->elementarySum(3);
	m1->getData()[3][0] = 10.0;
	cout << m1->getData()[2][0];
	cout << m1->getData()[3][0];
	m1->sum(m2);
	cout << m1->getData()[2][0];
	cout << m1->getData()[3][0];
	return 0;
}