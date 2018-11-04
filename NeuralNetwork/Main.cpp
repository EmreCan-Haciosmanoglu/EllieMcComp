#include "Matrix.h"
#include <iostream>
using namespace::std;
int main(int argc, char *argv[]) 
{
	Matrix m1(5, 6);
	Matrix m2(5, 6);
	m1.elementarySum(1);
	m2.elementarySum(1);
	m1.getData()[3][0] = 10.0;
	cout << m1.getData()[2][0];
	cout << m1.getData()[3][0];
	m1.sum(m2);
	cout << m1.getData()[2][0];
	cout << m1.getData()[3][0] << endl;
	for (int i = 0; i < m1.getRows(); i++)
	{
		for (int j = 0; j < m1.getColumns(); j++)
		{
			cout << m1.getData()[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	Matrix m3 = m1;
	m1.transpose();
	for (int i = 0; i < m1.getRows(); i++)
	{
		for (int j = 0; j < m1.getColumns(); j++)
		{
			cout << m1.getData()[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	m3.matrixMult(m1);
	for (int i = 0; i < m3.getRows(); i++)
	{
		for (int j = 0; j < m3.getColumns(); j++)
		{
			cout << m3.getData()[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}