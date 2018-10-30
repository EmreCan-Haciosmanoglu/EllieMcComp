#include "Matrix.h"
#include <iostream>
using namespace::std;
int main(int argc, char *argv[]) 
{
	Matrix* m = new Matrix(5, 6);
	cout << m->getData()[2][0];
	return 0;
}